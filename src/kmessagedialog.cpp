/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 2020 Ahmad Samir <a.samirh78@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "kmessagedialog.h"
#include "kmessagebox_p.h"

#include "loggingcategory.h"

#include <QApplication>
#include <QCheckBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QStyle>
#include <QStyleOption>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QWindow>

#include <KCollapsibleGroupBox>
#include <KSqueezedTextLabel>

static const Qt::TextInteractionFlags s_textFlags = Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse | Qt::LinksAccessibleByKeyboard;

// TODO KF6 remove QObject inheritance again
class KMessageDialogPrivate : public QObject
{
    Q_OBJECT

public:
    explicit KMessageDialogPrivate(KMessageDialog::Type type, KMessageDialog *qq)
        : m_type(type)
        , q(qq)
    {
    }

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (event->type() == QEvent::Show && watched == q) {
            Q_ASSERT(m_notifyEnabled);
            doNotify();
        }
        return false;
    }
#endif

    void doNotify()
    {
#ifndef Q_OS_WIN // FIXME problems with KNotify on Windows
        QMessageBox::Icon notifyType = QMessageBox::NoIcon;
        switch (m_type) {
        case KMessageDialog::QuestionYesNo:
        case KMessageDialog::QuestionYesNoCancel:
            notifyType = QMessageBox::Question;
            break;
        case KMessageDialog::WarningYesNo:
        case KMessageDialog::WarningYesNoCancel:
        case KMessageDialog::WarningContinueCancel:
        case KMessageDialog::Information:
            notifyType = QMessageBox::Information;
            break;
        case KMessageDialog::Error:
            notifyType = QMessageBox::Critical;
            break;
#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 97)
        case KMessageDialog::Sorry:
            notifyType = QMessageBox::Warning;
            break;
#endif
        }

        // TODO include m_listWidget items
        KMessageBox::notifyInterface()->sendNotification(notifyType, m_messageLabel->text(), q->topLevelWidget());
#endif
    }

    KMessageDialog::Type m_type;
    KMessageDialog *const q;

    QVBoxLayout *m_topLayout = nullptr;
    QWidget *m_mainWidget = nullptr;
    QLabel *m_iconLabel = nullptr;
    QLabel *m_messageLabel = nullptr;
    QListWidget *m_listWidget = nullptr;
    QTextBrowser *m_detailsTextEdit = nullptr;
    KCollapsibleGroupBox *m_detailsGroup = nullptr;
    QCheckBox *m_dontAskAgainCB = nullptr;
    QDialogButtonBox *m_buttonBox = nullptr;
    QMetaObject::Connection m_buttonBoxConnection;
    bool m_notifyEnabled = true;
};

KMessageDialog::KMessageDialog(KMessageDialog::Type type, const QString &text, QWidget *parent)
    : QDialog(parent)
    , d(new KMessageDialogPrivate(type, this))
{
    // Dialog top-level layout
    d->m_topLayout = new QVBoxLayout(this);
    d->m_topLayout->setSizeConstraint(QLayout::SetFixedSize);

    // Main widget
    d->m_mainWidget = new QWidget(this);
    d->m_topLayout->addWidget(d->m_mainWidget);

    // Layout for the main widget
    auto *mainLayout = new QVBoxLayout(d->m_mainWidget);
    QStyle *widgetStyle = d->m_mainWidget->style();
    // Provide extra spacing
    mainLayout->setSpacing(widgetStyle->pixelMetric(QStyle::PM_LayoutVerticalSpacing) * 2);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    auto *hLayout = new QHBoxLayout{};
    mainLayout->addLayout(hLayout, 5);

    // Icon
    auto *iconLayout = new QVBoxLayout{};
    hLayout->addLayout(iconLayout, 0);

    d->m_iconLabel = new QLabel(d->m_mainWidget);
    d->m_iconLabel->setVisible(false);
    iconLayout->addWidget(d->m_iconLabel);
    hLayout->addSpacing(widgetStyle->pixelMetric(QStyle::PM_LayoutHorizontalSpacing));

    const QRect desktop = screen()->geometry();
    const auto desktopWidth = desktop.width();
    // Main message text
    d->m_messageLabel = new QLabel(text, d->m_mainWidget);
    if (d->m_messageLabel->sizeHint().width() > (desktopWidth * 0.5)) {
        // Enable automatic wrapping of messages which are longer than 50% of screen width
        d->m_messageLabel->setWordWrap(true);
        // Use a squeezed label if text is still too wide
        const bool usingSqueezedLabel = d->m_messageLabel->sizeHint().width() > (desktopWidth * 0.85);
        if (usingSqueezedLabel) {
            delete d->m_messageLabel;
            d->m_messageLabel = new KSqueezedTextLabel(text, d->m_mainWidget);
        }
    }

    d->m_messageLabel->setTextInteractionFlags(s_textFlags);

    const bool usingScrollArea = (desktop.height() / 3) < d->m_messageLabel->sizeHint().height();
    if (usingScrollArea) {
        QScrollArea *messageScrollArea = new QScrollArea(d->m_mainWidget);
        messageScrollArea->setWidget(d->m_messageLabel);
        messageScrollArea->setFrameShape(QFrame::NoFrame);
        messageScrollArea->setWidgetResizable(true);
        hLayout->addWidget(messageScrollArea, 5);
    } else {
        hLayout->addWidget(d->m_messageLabel, 5);
    }

    // List widget, will be populated by setListWidgetItems()
    d->m_listWidget = new QListWidget(d->m_mainWidget);
    mainLayout->addWidget(d->m_listWidget, usingScrollArea ? 10 : 50);
    d->m_listWidget->setVisible(false);

    // DontAskAgain checkbox, will be set up by setDontAskAgainText()
    d->m_dontAskAgainCB = new QCheckBox(d->m_mainWidget);
    mainLayout->addWidget(d->m_dontAskAgainCB);
    d->m_dontAskAgainCB->setVisible(false);

    // Details widget, text will be added by setDetails()
    auto *detailsHLayout = new QHBoxLayout{};
    d->m_topLayout->addLayout(detailsHLayout);

    d->m_detailsGroup = new KCollapsibleGroupBox();
    d->m_detailsGroup->setVisible(false);
    d->m_detailsGroup->setTitle(QApplication::translate("KMessageDialog", "Details"));
    QVBoxLayout *detailsLayout = new QVBoxLayout(d->m_detailsGroup);

    d->m_detailsTextEdit = new QTextBrowser{};
    d->m_detailsTextEdit->setMinimumHeight(d->m_detailsTextEdit->fontMetrics().lineSpacing() * 11);
    detailsLayout->addWidget(d->m_detailsTextEdit, 50);

    detailsHLayout->addWidget(d->m_detailsGroup);

    // Button box
    d->m_buttonBox = new QDialogButtonBox(this);
    d->m_topLayout->addWidget(d->m_buttonBox);

    // Default buttons
    setButtons();
    
    setNotifyEnabled(true);

    // If the dialog is rejected, e.g. by pressing Esc, done() signal connected to the button box
    // won't be emitted
    connect(this, &QDialog::rejected, this, [this]() {
        done(QDialogButtonBox::Cancel);
    });
}

// This method has been copied from KWindowSystem to avoid depending on it
static void setMainWindow(QDialog *dialog, WId mainWindowId)
{
#ifdef Q_OS_OSX
    if (!QWidget::find(mainWindowId)) {
        return;
    }
#endif
    // Set the WA_NativeWindow attribute to force the creation of the QWindow.
    // Without this QWidget::windowHandle() returns 0.
    dialog->setAttribute(Qt::WA_NativeWindow, true);
    QWindow *subWindow = dialog->windowHandle();
    Q_ASSERT(subWindow);

    QWindow *mainWindow = QWindow::fromWinId(mainWindowId);
    if (!mainWindow) {
        // foreign windows not supported on all platforms
        return;
    }
    // mainWindow is not the child of any object, so make sure it gets deleted at some point
    QObject::connect(dialog, &QObject::destroyed, mainWindow, &QObject::deleteLater);
    subWindow->setTransientParent(mainWindow);
}

KMessageDialog::KMessageDialog(KMessageDialog::Type type, const QString &text, WId parent_id)
    : KMessageDialog(type, text)
{
    QWidget *parent = QWidget::find(parent_id);
    setParent(parent);
    if (!parent && parent_id) {
        setMainWindow(this, parent_id);
    }
}

KMessageDialog::~KMessageDialog()
{
    removeEventFilter(d.get());
}

void KMessageDialog::setCaption(const QString &caption)
{
    if (!caption.isEmpty()) {
        setWindowTitle(caption);
        return;
    }

    QString title;
    switch (d->m_type) { // Get a title based on the dialog Type
    case KMessageDialog::QuestionYesNo:
    case KMessageDialog::QuestionYesNoCancel:
        title = QApplication::translate("KMessageDialog", "Question");
        break;
    case KMessageDialog::WarningYesNo:
    case KMessageDialog::WarningYesNoCancel:
    case KMessageDialog::WarningContinueCancel:
        title = QApplication::translate("KMessageDialog", "Warning");
        break;
    case KMessageDialog::Information:
        title = QApplication::translate("KMessageDialog", "Information");
        break;
#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 97)
    case KMessageDialog::Sorry:
        title = QApplication::translate("KMessageDialog", "Sorry");
        break;
#endif
    case KMessageDialog::Error: {
        title = QApplication::translate("KMessageDialog", "Error");
        break;
    }
    default:
        break;
    }

    setWindowTitle(title);
}

void KMessageDialog::setIcon(const QIcon &icon)
{
    QIcon effectiveIcon(icon);
    if (effectiveIcon.isNull()) { // Fallback to an icon based on the dialog Type
        QStyle *style = this->style();
        switch (d->m_type) {
        case KMessageDialog::QuestionYesNo:
        case KMessageDialog::QuestionYesNoCancel:
            effectiveIcon = style->standardIcon(QStyle::SP_MessageBoxQuestion, nullptr, this);
            break;
        case KMessageDialog::WarningYesNo:
        case KMessageDialog::WarningYesNoCancel:
        case KMessageDialog::WarningContinueCancel:
#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 97)
        case KMessageDialog::Sorry:
#endif
            effectiveIcon = style->standardIcon(QStyle::SP_MessageBoxWarning, nullptr, this);
            break;
        case KMessageDialog::Information:
            effectiveIcon = style->standardIcon(QStyle::SP_MessageBoxInformation, nullptr, this);
            break;
        case KMessageDialog::Error:
            effectiveIcon = style->standardIcon(QStyle::SP_MessageBoxCritical, nullptr, this);
            break;
        default:
            break;
        }
    }

    if (effectiveIcon.isNull()) {
        qCWarning(KWidgetsAddonsLog) << "Neither the requested icon nor a generic one based on the "
                                        "dialog type could be found.";
        return;
    }

    d->m_iconLabel->setVisible(true);

    QStyleOption option;
    option.initFrom(d->m_mainWidget);
    QStyle *widgetStyle = d->m_mainWidget->style();
    const int size = widgetStyle->pixelMetric(QStyle::PM_MessageBoxIconSize, &option, d->m_mainWidget);
    d->m_iconLabel->setPixmap(effectiveIcon.pixmap(size));
}

void KMessageDialog::setListWidgetItems(const QStringList &strlist)
{
    const bool isEmpty = strlist.isEmpty();
    d->m_listWidget->setVisible(!isEmpty);
    if (isEmpty) {
        return;
    }

    // Enable automatic wrapping since the listwidget already has a good initial width
    d->m_messageLabel->setWordWrap(true);
    d->m_listWidget->addItems(strlist);

    QStyleOptionViewItem styleOption;
    styleOption.initFrom(d->m_listWidget);
    QFontMetrics fm(styleOption.font);
    int listWidth = d->m_listWidget->width();
    for (const QString &str : strlist) {
        listWidth = qMax(listWidth, fm.boundingRect(str).width());
    }
    const int borderWidth = (d->m_listWidget->width() - d->m_listWidget->viewport()->width() //
                             + d->m_listWidget->verticalScrollBar()->height());
    listWidth += borderWidth;
    const auto deskWidthPortion = screen()->geometry().width() * 0.85;
    if (listWidth > deskWidthPortion) { // Limit the list widget size to 85% of screen width
        listWidth = qRound(deskWidthPortion);
    }
    d->m_listWidget->setMinimumWidth(listWidth);
    d->m_listWidget->setSelectionMode(QListWidget::NoSelection);
    d->m_messageLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
}

void KMessageDialog::setDetails(const QString &details)
{
    d->m_detailsGroup->setVisible(!details.isEmpty());
    d->m_detailsTextEdit->setText(details);
}

void KMessageDialog::setButtons(const KGuiItem &buttonAccept, const KGuiItem &buttonNo, const KGuiItem &buttonCancel)
{
    switch (d->m_type) {
    case KMessageDialog::QuestionYesNo: {
        d->m_buttonBox->setStandardButtons(QDialogButtonBox::Yes | QDialogButtonBox::No);
        auto *buttonYes = d->m_buttonBox->button(QDialogButtonBox::Yes);
        KGuiItem::assign(buttonYes, buttonAccept);
        buttonYes->setFocus();
        KGuiItem::assign(d->m_buttonBox->button(QDialogButtonBox::No), buttonNo);
        break;
    }
    case KMessageDialog::QuestionYesNoCancel: {
        d->m_buttonBox->setStandardButtons(QDialogButtonBox::Yes | QDialogButtonBox::No | QDialogButtonBox::Cancel);
        auto *buttonYes = d->m_buttonBox->button(QDialogButtonBox::Yes);
        KGuiItem::assign(buttonYes, buttonAccept);
        buttonYes->setFocus();
        KGuiItem::assign(d->m_buttonBox->button(QDialogButtonBox::No), buttonNo);
        KGuiItem::assign(d->m_buttonBox->button(QDialogButtonBox::Cancel), buttonCancel);
        break;
    }
    case KMessageDialog::WarningYesNo: {
        d->m_buttonBox->setStandardButtons(QDialogButtonBox::Yes | QDialogButtonBox::No);
        KGuiItem::assign(d->m_buttonBox->button(QDialogButtonBox::Yes), buttonAccept);

        auto *noBtn = d->m_buttonBox->button(QDialogButtonBox::No);
        KGuiItem::assign(noBtn, buttonNo);
        noBtn->setDefault(true);
        noBtn->setFocus();
        break;
    }
    case KMessageDialog::WarningYesNoCancel: {
        d->m_buttonBox->setStandardButtons(QDialogButtonBox::Yes | QDialogButtonBox::No | QDialogButtonBox::Cancel);
        KGuiItem::assign(d->m_buttonBox->button(QDialogButtonBox::Yes), buttonAccept);
        KGuiItem::assign(d->m_buttonBox->button(QDialogButtonBox::No), buttonNo);

        auto *cancelButton = d->m_buttonBox->button(QDialogButtonBox::Cancel);
        KGuiItem::assign(cancelButton, buttonCancel);
        cancelButton->setDefault(true);
        cancelButton->setFocus();
        break;
    }
    case KMessageDialog::WarningContinueCancel: {
        d->m_buttonBox->setStandardButtons(QDialogButtonBox::Yes | QDialogButtonBox::Cancel);

        KGuiItem buttonContinue = buttonAccept;
        if (buttonContinue.text() == KStandardGuiItem::yes().text()) {
            buttonContinue = KStandardGuiItem::cont();
        }
        KGuiItem::assign(d->m_buttonBox->button(QDialogButtonBox::Yes), buttonContinue);

        auto *cancelButton = d->m_buttonBox->button(QDialogButtonBox::Cancel);
        KGuiItem::assign(cancelButton, buttonCancel);
        cancelButton->setDefault(true);
        cancelButton->setFocus();
        break;
    }
    case KMessageDialog::Information:
#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 97)
    case KMessageDialog::Sorry:
#endif
    case KMessageDialog::Error: {
        d->m_buttonBox->setStandardButtons(QDialogButtonBox::Ok);
        auto *okButton = d->m_buttonBox->button(QDialogButtonBox::Ok);
        KGuiItem::assign(okButton, KStandardGuiItem::ok());
        okButton->setFocus();
        break;
    }
    default:
        break;
    }

    // Button connections
    if (!d->m_buttonBoxConnection) {
        d->m_buttonBoxConnection = connect(d->m_buttonBox, &QDialogButtonBox::clicked, this, [this](QAbstractButton *button) {
            QDialogButtonBox::StandardButton code = d->m_buttonBox->standardButton(button);
            if (code != QDialogButtonBox::NoButton) {
                done(code);
            }
        });
    }
}

void KMessageDialog::setDontAskAgainText(const QString &dontAskAgainText)
{
    d->m_dontAskAgainCB->setVisible(!dontAskAgainText.isEmpty());
    d->m_dontAskAgainCB->setText(dontAskAgainText);
}

void KMessageDialog::setDontAskAgainChecked(bool isChecked)
{
    if (d->m_dontAskAgainCB->text().isEmpty()) {
        qCWarning(KWidgetsAddonsLog) << "setDontAskAgainChecked() method was called on a dialog that doesn't "
                                        "appear to have a checkbox; you need to use setDontAskAgainText() "
                                        "to add a checkbox to the dialog first.";
        return;
    }

    d->m_dontAskAgainCB->setChecked(isChecked);
}

bool KMessageDialog::isDontAskAgainChecked() const
{
    if (d->m_dontAskAgainCB->text().isEmpty()) {
        qCWarning(KWidgetsAddonsLog) << "isDontAskAgainChecked() method was called on a dialog that doesn't "
                                        "appear to have a checkbox; you need to use setDontAskAgainText() "
                                        "to add a checkbox to the dialog first.";
        return false;
    }

    return d->m_dontAskAgainCB->isChecked();
}

void KMessageDialog::setOpenExternalLinks(bool isAllowed)
{
    d->m_messageLabel->setOpenExternalLinks(isAllowed);
    d->m_detailsTextEdit->setOpenExternalLinks(isAllowed);
}

bool KMessageDialog::isNotifyEnabled() const
{
    return d->m_notifyEnabled;
}

void KMessageDialog::setNotifyEnabled(bool enable)
{
    d->m_notifyEnabled = enable;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    if (enable) {
        installEventFilter(d.get());
    } else {
        removeEventFilter(d.get());
    }
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void KMessageDialog::showEvent(QShowEvent *event)
{
    if (d->m_notifyEnabled) {
        d->doNotify();
    }
    QDialog::showEvent(event);
}
#endif

#include "kmessagedialog.moc"
