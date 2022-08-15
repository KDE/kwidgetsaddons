/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2015 David Edmundson <davidedmundson@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kcollapsiblegroupbox.h"

#include <QLabel>
#include <QLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QTimeLine>

class KCollapsibleGroupBoxPrivate
{
public:
    KCollapsibleGroupBoxPrivate(KCollapsibleGroupBox *qq);
    void updateChildrenFocus(bool expanded);
    void recalculateHeaderSize();
    QSize contentSize() const;
    QSize contentMinimumSize() const;

    KCollapsibleGroupBox *const q;
    QTimeLine *animation;
    QString title;
    bool isExpanded = false;
    bool headerContainsMouse = false;
    QSize headerSize;
    int shortcutId = 0;
    QMap<QWidget *, Qt::FocusPolicy> focusMap; // Used to restore focus policy of widgets.
};

KCollapsibleGroupBoxPrivate::KCollapsibleGroupBoxPrivate(KCollapsibleGroupBox *qq)
    : q(qq)
{
}

KCollapsibleGroupBox::KCollapsibleGroupBox(QWidget *parent)
    : QWidget(parent)
    , d(new KCollapsibleGroupBoxPrivate(this))
{
    d->recalculateHeaderSize();

    d->animation = new QTimeLine(500, this); // duration matches kmessagewidget
    connect(d->animation, &QTimeLine::valueChanged, this, [this](qreal value) {
        setFixedHeight((d->contentSize().height() * value) + d->headerSize.height());
    });
    connect(d->animation, &QTimeLine::stateChanged, this, [this](QTimeLine::State state) {
        if (state == QTimeLine::NotRunning) {
            d->updateChildrenFocus(d->isExpanded);
        }
    });

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    setFocusPolicy(Qt::TabFocus);
    setMouseTracking(true);
}

KCollapsibleGroupBox::~KCollapsibleGroupBox()
{
    if (d->animation->state() == QTimeLine::Running) {
        d->animation->stop();
    }
}

void KCollapsibleGroupBox::setTitle(const QString &title)
{
    d->title = title;
    d->recalculateHeaderSize();

    update();
    updateGeometry();

    if (d->shortcutId) {
        releaseShortcut(d->shortcutId);
    }

    d->shortcutId = grabShortcut(QKeySequence::mnemonic(title));

#ifndef QT_NO_ACCESSIBILITY
    setAccessibleName(title);
#endif

    Q_EMIT titleChanged();
}

QString KCollapsibleGroupBox::title() const
{
    return d->title;
}

void KCollapsibleGroupBox::setExpanded(bool expanded)
{
    if (expanded == d->isExpanded) {
        return;
    }

    d->isExpanded = expanded;
    Q_EMIT expandedChanged();

    d->updateChildrenFocus(expanded);

    d->animation->setDirection(expanded ? QTimeLine::Forward : QTimeLine::Backward);
    // QTimeLine::duration() must be > 0
    const int duration = qMax(1, style()->styleHint(QStyle::SH_Widget_Animation_Duration));
    d->animation->stop();
    d->animation->setDuration(duration);
    d->animation->start();

    // when going from collapsed to expanded changing the child visibility calls an updateGeometry
    // which calls sizeHint with expanded true before the first frame of the animation kicks in
    // trigger an effective frame 0
    if (expanded) {
        setFixedHeight(d->headerSize.height());
    }
}

bool KCollapsibleGroupBox::isExpanded() const
{
    return d->isExpanded;
}

void KCollapsibleGroupBox::collapse()
{
    setExpanded(false);
}

void KCollapsibleGroupBox::expand()
{
    setExpanded(true);
}

void KCollapsibleGroupBox::toggle()
{
    setExpanded(!d->isExpanded);
}

void KCollapsibleGroupBox::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    QStyleOptionButton baseOption;
    baseOption.initFrom(this);
    baseOption.rect = QRect(0, 0, width(), d->headerSize.height());
    baseOption.text = d->title;

    if (d->headerContainsMouse) {
        baseOption.state |= QStyle::State_MouseOver;
    }

    QStyle::PrimitiveElement element;
    if (d->isExpanded) {
        element = QStyle::PE_IndicatorArrowDown;
    } else {
        element = isLeftToRight() ? QStyle::PE_IndicatorArrowRight : QStyle::PE_IndicatorArrowLeft;
    }

    QStyleOptionButton indicatorOption = baseOption;
    indicatorOption.rect = style()->subElementRect(QStyle::SE_CheckBoxIndicator, &indicatorOption, this);
    style()->drawPrimitive(element, &indicatorOption, &p, this);

    QStyleOptionButton labelOption = baseOption;
    labelOption.rect = style()->subElementRect(QStyle::SE_CheckBoxContents, &labelOption, this);
    style()->drawControl(QStyle::CE_CheckBoxLabel, &labelOption, &p, this);

    Q_UNUSED(event)
}

QSize KCollapsibleGroupBox::sizeHint() const
{
    if (d->isExpanded) {
        return d->contentSize() + QSize(0, d->headerSize.height());
    } else {
        return QSize(d->contentSize().width(), d->headerSize.height());
    }
}

QSize KCollapsibleGroupBox::minimumSizeHint() const
{
    int minimumWidth = qMax(d->contentSize().width(), d->headerSize.width());
    return QSize(minimumWidth, d->headerSize.height());
}

bool KCollapsibleGroupBox::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::StyleChange:
        /*fall through*/
    case QEvent::FontChange:
        d->recalculateHeaderSize();
        break;
    case QEvent::Shortcut: {
        QShortcutEvent *se = static_cast<QShortcutEvent *>(event);
        if (d->shortcutId == se->shortcutId()) {
            toggle();
            return true;
        }
        break;
    }
    case QEvent::ChildAdded: {
        QChildEvent *ce = static_cast<QChildEvent *>(event);
        if (ce->child()->isWidgetType()) {
            auto widget = static_cast<QWidget *>(ce->child());
            // Needs to be called asynchronously because at this point the widget is likely a "real" QWidget,
            // i.e. the QWidget base class whose constructor sets the focus policy to NoPolicy.
            // But the constructor of the child class (not yet called) could set a different focus policy later.
            auto focusFunc = [this, widget]() {
                overrideFocusPolicyOf(widget);
            };
            QMetaObject::invokeMethod(this, focusFunc, Qt::QueuedConnection);
        }
        break;
    }
    case QEvent::LayoutRequest:
        if (d->animation->state() == QTimeLine::NotRunning) {
            setFixedHeight(sizeHint().height());
        }
        break;
    default:
        break;
    }

    return QWidget::event(event);
}

void KCollapsibleGroupBox::mousePressEvent(QMouseEvent *event)
{
    const QRect headerRect(0, 0, width(), d->headerSize.height());
    if (headerRect.contains(event->pos())) {
        toggle();
    }
    event->setAccepted(true);
}

// if mouse has changed whether it is in the top bar or not refresh to change arrow icon
void KCollapsibleGroupBox::mouseMoveEvent(QMouseEvent *event)
{
    const QRect headerRect(0, 0, width(), d->headerSize.height());
    bool headerContainsMouse = headerRect.contains(event->pos());

    if (headerContainsMouse != d->headerContainsMouse) {
        d->headerContainsMouse = headerContainsMouse;
        update();
    }

    QWidget::mouseMoveEvent(event);
}

void KCollapsibleGroupBox::leaveEvent(QEvent *event)
{
    d->headerContainsMouse = false;
    update();
    QWidget::leaveEvent(event);
}

void KCollapsibleGroupBox::keyPressEvent(QKeyEvent *event)
{
    // event might have just propagated up from a child, if so we don't want to react to it
    if (!hasFocus()) {
        return;
    }
    const int key = event->key();
    if (key == Qt::Key_Space || key == Qt::Key_Enter || key == Qt::Key_Return) {
        toggle();
        event->setAccepted(true);
    }
}

void KCollapsibleGroupBox::resizeEvent(QResizeEvent *event)
{
    const QMargins margins = contentsMargins();

    if (layout()) {
        // we don't want the layout trying to fit the current frame of the animation so always set it to the target height
        layout()->setGeometry(QRect(margins.left(), margins.top(), width() - margins.left() - margins.right(), layout()->sizeHint().height()));
    }

    QWidget::resizeEvent(event);
}

void KCollapsibleGroupBox::overrideFocusPolicyOf(QWidget *widget)
{
    d->focusMap.insert(widget, widget->focusPolicy());

    if (!isExpanded()) {
        // Prevent tab focus if not expanded.
        widget->setFocusPolicy(Qt::NoFocus);
    }
}

void KCollapsibleGroupBoxPrivate::recalculateHeaderSize()
{
    QStyleOption option;
    option.initFrom(q);

    QSize textSize = q->style()->itemTextRect(option.fontMetrics, QRect(), Qt::TextShowMnemonic, false, title).size();

    headerSize = q->style()->sizeFromContents(QStyle::CT_CheckBox, &option, textSize, q);
    q->setContentsMargins(q->style()->pixelMetric(QStyle::PM_IndicatorWidth), headerSize.height(), 0, 0);
}

void KCollapsibleGroupBoxPrivate::updateChildrenFocus(bool expanded)
{
    const auto children = q->children();
    for (QObject *child : children) {
        QWidget *widget = qobject_cast<QWidget *>(child);
        if (!widget) {
            continue;
        }
        // Restore old focus policy if expanded, remove from focus chain otherwise.
        if (expanded) {
            widget->setFocusPolicy(focusMap.value(widget));
        } else {
            widget->setFocusPolicy(Qt::NoFocus);
        }
    }
}

QSize KCollapsibleGroupBoxPrivate::contentSize() const
{
    if (q->layout()) {
        const QMargins margins = q->contentsMargins();
        const QSize marginSize(margins.left() + margins.right(), margins.top() + margins.bottom());
        return q->layout()->sizeHint() + marginSize;
    }
    return QSize(0, 0);
}

QSize KCollapsibleGroupBoxPrivate::contentMinimumSize() const
{
    if (q->layout()) {
        const QMargins margins = q->contentsMargins();
        const QSize marginSize(margins.left() + margins.right(), margins.top() + margins.bottom());
        return q->layout()->minimumSize() + marginSize;
    }
    return QSize(0, 0);
}
