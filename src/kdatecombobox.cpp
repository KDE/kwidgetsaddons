/*
    SPDX-FileCopyrightText: 2011 John Layt <john@layt.net>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kdatecombobox.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QDate>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMenu>
#include <QScreen>
#include <QVector>
#include <QWidgetAction>

#include "kdatepicker.h"
#include "kmessagebox.h"

class KDateComboBoxPrivate
{
public:
    KDateComboBoxPrivate(KDateComboBox *q);
    virtual ~KDateComboBoxPrivate();

    // TODO: Find a way to get that from QLocale
#if 0
    QDate defaultMinDate();
    QDate defaultMaxDate();
#endif

    QString dateFormat(QLocale::FormatType format);
    QString formatDate(const QDate &date);

    void initDateWidget();
    void addMenuAction(const QString &text, const QDate &date);
    void enableMenuDates();
    void updateDateWidget();
    void setDateRange(const QDate &minDate, const QDate &maxDate, const QString &minWarnMsg, const QString &maxWarnMsg);
    bool isInDateRange(const QDate &date) const;

    void clickDate();
    void selectDate(QAction *action);
    void editDate(const QString &text);
    void enterDate(const QDate &date);
    void parseDate();
    void warnDate();

    KDateComboBox *const q;
    QMenu *m_dateMenu;
    QVector<QAction *> m_actions;
    KDatePicker *m_datePicker;
    QWidgetAction *m_datePickerAction;

    QDate m_date;
    KDateComboBox::Options m_options;
    QDate m_minDate;
    QDate m_maxDate;
    QString m_minWarnMsg;
    QString m_maxWarnMsg;
    bool m_warningShown;
    bool m_edited; // and dateChanged not yet emitted
    QLocale::FormatType m_displayFormat;
    QMap<QDate, QString> m_dateMap;
};

KDateComboBoxPrivate::KDateComboBoxPrivate(KDateComboBox *q)
    : q(q)
    , m_dateMenu(new QMenu(q))
    , m_datePicker(new KDatePicker(q))
    , m_datePickerAction(new QWidgetAction(q))
    , m_warningShown(false)
    , m_edited(false)
    , m_displayFormat(QLocale::ShortFormat)
{
    m_options = KDateComboBox::EditDate | KDateComboBox::SelectDate | KDateComboBox::DatePicker | KDateComboBox::DateKeywords;
    m_date = QDate::currentDate();
    // m_minDate = defaultMinDate();
    // m_maxDate = defaultMaxDate();
    m_datePicker->setCloseButton(false);
    m_datePickerAction->setObjectName(QStringLiteral("DatePicker"));
    m_datePickerAction->setDefaultWidget(m_datePicker);
}

KDateComboBoxPrivate::~KDateComboBoxPrivate()
{
}

#if 0
QDate KDateComboBoxPrivate::defaultMinDate()
{
    return m_date.calendar()->earliestValidDate();
}

QDate KDateComboBoxPrivate::defaultMaxDate()
{
    return m_date.calendar()->latestValidDate();
}
#endif

QString KDateComboBoxPrivate::dateFormat(QLocale::FormatType format)
{
    // Clearly a workaround for QLocale using "yy" way too often for years
    // and so you get no way to distinguish between 1913 and 2013 anymore...
    // bummer.
    QString res = q->locale().dateFormat(format);
    res.replace(QLatin1String("yy"), QLatin1String("yyyy"));
    res.replace(QLatin1String("yyyyyyyy"), QLatin1String("yyyy"));
    return res;
}

QString KDateComboBoxPrivate::formatDate(const QDate &date)
{
    return q->locale().toString(date, dateFormat(m_displayFormat));
}

void KDateComboBoxPrivate::initDateWidget()
{
    q->blockSignals(true);
    q->clear();

    // If EditTime then set the line edit
    q->lineEdit()->setReadOnly((m_options & KDateComboBox::EditDate) != KDateComboBox::EditDate);

    // If SelectDate then make list items visible
    if ((m_options & KDateComboBox::SelectDate) == KDateComboBox::SelectDate //
        || (m_options & KDateComboBox::DatePicker) == KDateComboBox::DatePicker //
        || (m_options & KDateComboBox::DatePicker) == KDateComboBox::DateKeywords) {
        q->setMaxVisibleItems(1);
    } else {
        q->setMaxVisibleItems(0);
    }

    q->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    q->addItem(formatDate(m_date));
    q->setCurrentIndex(0);
    q->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
    q->blockSignals(false);

    m_dateMenu->clear();
    m_actions.clear();

    if ((m_options & KDateComboBox::SelectDate) == KDateComboBox::SelectDate) {
        if ((m_options & KDateComboBox::DatePicker) == KDateComboBox::DatePicker) {
            m_dateMenu->addAction(m_datePickerAction);
            m_dateMenu->addSeparator();
        }

        if ((m_options & KDateComboBox::DateKeywords) == KDateComboBox::DateKeywords) {
            if (m_dateMap.isEmpty()) {
                addMenuAction(KDateComboBox::tr("Next Year", "@option next year"), m_date.addYears(1));
                addMenuAction(KDateComboBox::tr("Next Month", "@option next month"), m_date.addMonths(1));
                addMenuAction(KDateComboBox::tr("Next Week", "@option next week"), m_date.addDays(7));
                addMenuAction(KDateComboBox::tr("Tomorrow", "@option tomorrow"), m_date.addDays(1));
                addMenuAction(KDateComboBox::tr("Today", "@option today"), m_date);
                addMenuAction(KDateComboBox::tr("Yesterday", "@option yesterday"), m_date.addDays(-1));
                addMenuAction(KDateComboBox::tr("Last Week", "@option last week"), m_date.addDays(-7));
                addMenuAction(KDateComboBox::tr("Last Month", "@option last month"), m_date.addMonths(-1));
                addMenuAction(KDateComboBox::tr("Last Year", "@option last year"), m_date.addYears(-1));
                m_dateMenu->addSeparator();
                addMenuAction(KDateComboBox::tr("No Date", "@option do not specify a date"), QDate());

            } else {
                QMapIterator<QDate, QString> i(m_dateMap);
                while (i.hasNext()) {
                    i.next();
                    if (i.value().isEmpty()) {
                        addMenuAction(formatDate(i.key()), i.key());
                    } else if (i.value().toLower() == QLatin1String("separator")) {
                        m_dateMenu->addSeparator();
                    } else {
                        addMenuAction(i.value(), i.key());
                    }
                }
            }
            enableMenuDates();
        }
    }
}

void KDateComboBoxPrivate::addMenuAction(const QString &text, const QDate &date)
{
    QAction *action = new QAction(m_dateMenu);
    action->setText(text);
    action->setData(date);
    m_dateMenu->addAction(action);
    m_actions << action;
}

void KDateComboBoxPrivate::enableMenuDates()
{
    // Hide menu dates if they are outside the date range
    for (int i = 0; i < m_actions.count(); ++i) {
        QDate date = m_actions[i]->data().toDate();
        m_actions[i]->setVisible(!date.isValid() || isInDateRange(date));
    }
}

void KDateComboBoxPrivate::updateDateWidget()
{
    q->blockSignals(true);
    m_datePicker->blockSignals(true);
    m_datePicker->setDate(m_date);
    int pos = q->lineEdit()->cursorPosition();
    q->setItemText(0, formatDate(m_date));
    q->lineEdit()->setText(formatDate(m_date));
    q->lineEdit()->setCursorPosition(pos);
    m_datePicker->blockSignals(false);
    q->blockSignals(false);
}

void KDateComboBoxPrivate::setDateRange(const QDate &minDate, const QDate &maxDate, const QString &minWarnMsg, const QString &maxWarnMsg)
{
    if (minDate.isValid() && maxDate.isValid() && minDate > maxDate) {
        return;
    }

    if (minDate != m_minDate || maxDate != m_maxDate //
        || minWarnMsg != m_minWarnMsg || maxWarnMsg != m_maxWarnMsg) {
        m_minDate = minDate;
        m_maxDate = maxDate;
        m_minWarnMsg = minWarnMsg;
        m_maxWarnMsg = maxWarnMsg;
    }
    enableMenuDates();
}

bool KDateComboBoxPrivate::isInDateRange(const QDate &date) const
{
    return date.isValid() //
        && (!m_minDate.isValid() || date >= m_minDate) //
        && (!m_maxDate.isValid() || date <= m_maxDate);
}

void KDateComboBoxPrivate::selectDate(QAction *action)
{
    if (action->objectName() != QLatin1String("DatePicker")) {
        QDate date = action->data().toDate();
        if (isInDateRange(date)) {
            enterDate(date);
        }
    }
}

void KDateComboBoxPrivate::clickDate()
{
    QDate date = m_datePicker->date();
    if (isInDateRange(date)) {
        enterDate(date);
    }
}

void KDateComboBoxPrivate::editDate(const QString &text)
{
    m_warningShown = false;
    m_date = q->locale().toDate(text, dateFormat(m_displayFormat));
    m_edited = true;
    Q_EMIT q->dateEdited(m_date);
}

void KDateComboBoxPrivate::parseDate()
{
    m_date = q->locale().toDate(q->lineEdit()->text(), dateFormat(m_displayFormat));
}

void KDateComboBoxPrivate::enterDate(const QDate &date)
{
    q->setDate(date);
    // Re-add the combo box item in order to retain the correct widget width
    q->blockSignals(true);
    q->clear();
    q->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    q->addItem(formatDate(m_date));
    q->setCurrentIndex(0);
    q->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
    q->blockSignals(false);

    m_dateMenu->hide();
    warnDate();
    Q_EMIT q->dateEntered(m_date);
}

void KDateComboBoxPrivate::warnDate()
{
    if (!m_warningShown && !q->isValid() && (m_options & KDateComboBox::WarnOnInvalid) == KDateComboBox::WarnOnInvalid) {
        QString warnMsg;
        if (!m_date.isValid()) {
            warnMsg = KDateComboBox::tr("The date you entered is invalid", "@info");
        } else if (m_minDate.isValid() && m_date < m_minDate) {
            if (m_minWarnMsg.isEmpty()) {
                warnMsg = KDateComboBox::tr("Date cannot be earlier than %1", "@info").arg(formatDate(m_minDate));
            } else {
                warnMsg = m_minWarnMsg;
                warnMsg.replace(QLatin1String("%1"), formatDate(m_minDate));
            }
        } else if (m_maxDate.isValid() && m_date > m_maxDate) {
            if (m_maxWarnMsg.isEmpty()) {
                warnMsg = KDateComboBox::tr("Date cannot be later than %1", "@info").arg(formatDate(m_maxDate));
            } else {
                warnMsg = m_maxWarnMsg;
                warnMsg.replace(QLatin1String("%1"), formatDate(m_maxDate));
            }
        }
        m_warningShown = true;
        KMessageBox::sorry(q, warnMsg);
    }
}

KDateComboBox::KDateComboBox(QWidget *parent)
    : QComboBox(parent)
    , d(new KDateComboBoxPrivate(this))
{
    setEditable(true);
    setMaxVisibleItems(1);
    setInsertPolicy(QComboBox::NoInsert);
    d->m_datePicker->installEventFilter(this);
    d->initDateWidget();
    d->updateDateWidget();

    connect(d->m_dateMenu, &QMenu::triggered, this, [this](QAction *action) {
        d->selectDate(action);
    });

    connect(this, &QComboBox::editTextChanged, this, [this](const QString &text) {
        d->editDate(text);
    });

    connect(lineEdit(), &QLineEdit::returnPressed, this, [this]() {
        if (d->m_edited) {
            d->enterDate(date());
            Q_EMIT dateChanged(date());
        }
    });

    connect(d->m_datePicker, &KDatePicker::dateEntered, this, [this](const QDate &date) {
        d->enterDate(date);
    });
    connect(d->m_datePicker, &KDatePicker::tableClicked, this, [this]() {
        d->clickDate();
    });
}

KDateComboBox::~KDateComboBox() = default;

QDate KDateComboBox::date() const
{
    d->parseDate();
    return d->m_date;
}

void KDateComboBox::setDate(const QDate &date)
{
    if (date == d->m_date) {
        return;
    }

    d->m_edited = false;
    assignDate(date);
    d->updateDateWidget();
    Q_EMIT dateChanged(d->m_date);
}

void KDateComboBox::assignDate(const QDate &date)
{
    d->m_date = date;
}

bool KDateComboBox::isValid() const
{
    d->parseDate();
    return d->isInDateRange(d->m_date);
}

bool KDateComboBox::isNull() const
{
    return lineEdit()->text().isEmpty();
}

KDateComboBox::Options KDateComboBox::options() const
{
    return d->m_options;
}

void KDateComboBox::setOptions(Options options)
{
    if (options != d->m_options) {
        d->m_options = options;
        d->initDateWidget();
        d->updateDateWidget();
    }
}

QDate KDateComboBox::minimumDate() const
{
    return d->m_minDate;
}

void KDateComboBox::setMinimumDate(const QDate &minDate, const QString &minWarnMsg)
{
    if (minDate.isValid()) {
        d->setDateRange(minDate, d->m_maxDate, minWarnMsg, d->m_maxWarnMsg);
    }
}

void KDateComboBox::resetMinimumDate()
{
    d->setDateRange(QDate(), d->m_maxDate, QString(), d->m_maxWarnMsg);
}

QDate KDateComboBox::maximumDate() const
{
    return d->m_maxDate;
}

void KDateComboBox::setMaximumDate(const QDate &maxDate, const QString &maxWarnMsg)
{
    if (maxDate.isValid()) {
        d->setDateRange(d->m_minDate, maxDate, d->m_minWarnMsg, maxWarnMsg);
    }
}

void KDateComboBox::resetMaximumDate()
{
    d->setDateRange(d->m_minDate, QDate(), d->m_minWarnMsg, QString());
}

void KDateComboBox::setDateRange(const QDate &minDate, const QDate &maxDate, const QString &minWarnMsg, const QString &maxWarnMsg)
{
    if (minDate.isValid() && maxDate.isValid()) {
        d->setDateRange(minDate, maxDate, minWarnMsg, maxWarnMsg);
    }
}

void KDateComboBox::resetDateRange()
{
    d->setDateRange(QDate(), QDate(), QString(), QString());
}

QLocale::FormatType KDateComboBox::displayFormat() const
{
    return d->m_displayFormat;
}

void KDateComboBox::setDisplayFormat(QLocale::FormatType format)
{
    if (format != d->m_displayFormat) {
        d->m_displayFormat = format;
        d->initDateWidget();
        d->updateDateWidget();
    }
}

QMap<QDate, QString> KDateComboBox::dateMap() const
{
    return d->m_dateMap;
}

void KDateComboBox::setDateMap(QMap<QDate, QString> dateMap)
{
    if (dateMap != d->m_dateMap) {
        d->m_dateMap.clear();
        d->m_dateMap = dateMap;
        d->initDateWidget();
    }
}

bool KDateComboBox::eventFilter(QObject *object, QEvent *event)
{
    return QComboBox::eventFilter(object, event);
}

void KDateComboBox::keyPressEvent(QKeyEvent *keyEvent)
{
    QDate temp;
    switch (keyEvent->key()) {
    case Qt::Key_Down:
        temp = d->m_date.addDays(-1);
        break;
    case Qt::Key_Up:
        temp = d->m_date.addDays(1);
        break;
    case Qt::Key_PageDown:
        temp = d->m_date.addMonths(-1);
        break;
    case Qt::Key_PageUp:
        temp = d->m_date.addMonths(1);
        break;
    default:
        QComboBox::keyPressEvent(keyEvent);
        return;
    }
    if (d->isInDateRange(temp)) {
        d->enterDate(temp);
    }
}

void KDateComboBox::focusOutEvent(QFocusEvent *event)
{
    d->parseDate();
    d->warnDate();
    if (d->m_edited) {
        d->m_edited = false;
        Q_EMIT dateChanged(d->m_date);
    }
    QComboBox::focusOutEvent(event);
}

void KDateComboBox::showPopup()
{
    if (!isEditable() || !d->m_dateMenu //
        || (d->m_options & KDateComboBox::SelectDate) != KDateComboBox::SelectDate) {
        return;
    }

    d->m_datePicker->blockSignals(true);
    d->m_datePicker->setDate(d->m_date);
    d->m_datePicker->blockSignals(false);

    const QRect desk = screen()->geometry();

    QPoint popupPoint = mapToGlobal(QPoint(0, 0));

    const int dateFrameHeight = d->m_dateMenu->sizeHint().height();
    if (popupPoint.y() + height() + dateFrameHeight > desk.bottom()) {
        popupPoint.setY(popupPoint.y() - dateFrameHeight);
    } else {
        popupPoint.setY(popupPoint.y() + height());
    }

    const int dateFrameWidth = d->m_dateMenu->sizeHint().width();
    if (popupPoint.x() + dateFrameWidth > desk.right()) {
        popupPoint.setX(desk.right() - dateFrameWidth);
    }

    if (popupPoint.x() < desk.left()) {
        popupPoint.setX(desk.left());
    }

    if (popupPoint.y() < desk.top()) {
        popupPoint.setY(desk.top());
    }

    d->m_dateMenu->popup(popupPoint);
}

void KDateComboBox::hidePopup()
{
    QComboBox::hidePopup();
}

void KDateComboBox::mousePressEvent(QMouseEvent *event)
{
    QComboBox::mousePressEvent(event);
}

void KDateComboBox::wheelEvent(QWheelEvent *event)
{
    QDate temp;
    if (event->angleDelta().y() < 0) {
        temp = d->m_date.addDays(-1);
    } else {
        temp = d->m_date.addDays(1);
    }
    if (d->isInDateRange(temp)) {
        d->enterDate(temp);
    }
}

void KDateComboBox::focusInEvent(QFocusEvent *event)
{
    QComboBox::focusInEvent(event);
}

void KDateComboBox::resizeEvent(QResizeEvent *event)
{
    QComboBox::resizeEvent(event);
}

#include "moc_kdatecombobox.cpp"
