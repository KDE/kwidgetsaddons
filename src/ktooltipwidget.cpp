/*
 * This file is part of the KDE project
 * Copyright (C) 2008 Fredrik Höglund <fredrik@kde.org>
 * Copyright (C) 2008 Konstantin Heil <konst.heil@stud.uni-heidelberg.de>
 * Copyright (C) 2009 Peter Penz <peter.penz@gmx.at>
 * Copyright (C) 2017 Elvis Angelaccio <elvis.angelaccio@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ktooltipwidget.h"

#include <QApplication>
#include <QPaintEvent>
#include <QScreen>
#include <QStylePainter>
#include <QStyleOptionFrame>
#include <QTimer>
#include <QVBoxLayout>
#include <QWindow>

class KToolTipWidget::KToolTipWidgetPrivate
{
public:
    KToolTipWidgetPrivate(KToolTipWidget *parent)
        : q(parent),
          layout(nullptr),
          content(nullptr)
    {}

    void init();
    void addWidget(QWidget *widget);
    void removeWidget();
    void show(const QPoint &pos, QWindow *transientParent);
    QPoint centerBelow(const QRect &rect, QScreen *screen) const;

    KToolTipWidget *q;
    QTimer hideTimer;
    QVBoxLayout *layout;
    QWidget *content;
};

void KToolTipWidget::KToolTipWidgetPrivate::init()
{
    layout = new QVBoxLayout(q);

    hideTimer.setSingleShot(true);
    hideTimer.setInterval(500);

    connect(&hideTimer, &QTimer::timeout, q, &QWidget::hide);

    q->setAttribute(Qt::WA_TranslucentBackground);
    q->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
}

void KToolTipWidget::KToolTipWidgetPrivate::addWidget(QWidget *widget)
{
    removeWidget();
    content = widget;
    layout->addWidget(content);
}

void KToolTipWidget::KToolTipWidgetPrivate::removeWidget()
{
    layout->removeWidget(content);
}

void KToolTipWidget::KToolTipWidgetPrivate::show(const QPoint &pos, QWindow *transientParent)
{
    if (pos.isNull()) {
        return;
    }

    q->move(pos);
    q->windowHandle()->setTransientParent(transientParent);
    q->show();
}

QPoint KToolTipWidget::KToolTipWidgetPrivate::centerBelow(const QRect &rect, QScreen *screen) const
{
    // It must be assured that:
    // - the content is fully visible
    // - the content is not drawn inside rect

    const QSize size = content->sizeHint();
    const int margin = q->style()->pixelMetric(QStyle::PM_ToolTipLabelFrameWidth);
    const QRect screenGeometry = screen->geometry();

    const bool hasRoomToLeft  = (rect.left()   - size.width()  - margin >= screenGeometry.left());
    const bool hasRoomToRight = (rect.right()  + size.width()  + margin <= screenGeometry.right());
    const bool hasRoomAbove   = (rect.top()    - size.height() - margin >= screenGeometry.top());
    const bool hasRoomBelow   = (rect.bottom() + size.height() + margin <= screenGeometry.bottom());
    if (!hasRoomAbove && !hasRoomBelow && !hasRoomToLeft && !hasRoomToRight) {
        return QPoint();
    }

    int x = 0;
    int y = 0;
    if (hasRoomBelow || hasRoomAbove) {
        x = qMax(screenGeometry.left(), rect.center().x() - size.width() / 2);
        if (x + size.width() >= screenGeometry.right()) {
            x = screenGeometry.right() - size.width() + 1;
        }
        if (hasRoomBelow) {
            y = rect.bottom() + margin;
        } else {
            y = rect.top() - size.height() - margin;
        }
    } else {
        Q_ASSERT(hasRoomToLeft || hasRoomToRight);
        if (hasRoomToRight) {
            x = rect.right() + margin;
        } else {
            x = rect.left() - size.width() - margin;
        }
        // Put the tooltip at the bottom of the screen. The x-coordinate has already
        // been adjusted, so that no overlapping with rect occurs.
        y = screenGeometry.bottom() - size.height() + 1;
    }

    return QPoint(x, y);
}

KToolTipWidget::KToolTipWidget(QWidget *parent)
    : QWidget(parent), d(new KToolTipWidgetPrivate(this))
{
    d->init();
    // Tell the platform-specific window manager that we want blur behind.
    createWinId();
    windowHandle()->setProperty("ENABLE_BLUR_BEHIND_HINT", true);
}

KToolTipWidget::~KToolTipWidget()
{
}

void KToolTipWidget::showAt(const QPoint &pos, QWidget *content, QWindow *transientParent)
{
    d->addWidget(content);
    d->show(pos, transientParent);
}

void KToolTipWidget::showBelow(const QRect &rect, QWidget *content, QWindow *transientParent)
{
    d->addWidget(content);
    d->show(d->centerBelow(rect, transientParent->screen()), transientParent);
}

int KToolTipWidget::hideDelay() const
{
    return d->hideTimer.interval();
}

void KToolTipWidget::hideLater()
{
    if (!isVisible()) {
        return;
    }

    if (hideDelay() > 0) {
        d->hideTimer.start();
    } else {
        hide();
    }
}

void KToolTipWidget::setHideDelay(int delay)
{
    d->hideTimer.setInterval(delay);
}

void KToolTipWidget::enterEvent(QEvent *)
{
    // If the mouse hovers the tooltip, leave it visible.
    d->hideTimer.stop();
}

void KToolTipWidget::hideEvent(QHideEvent *)
{
    d->removeWidget();
    // Give time to the content widget to get his own hide event.
    QTimer::singleShot(0, this, &KToolTipWidget::hidden);
}

void KToolTipWidget::leaveEvent(QEvent *)
{
    // Don't bother starting the hide timer, we are done.
    hide();
}

void KToolTipWidget::paintEvent(QPaintEvent *event)
{
    QStylePainter painter(this);
    painter.setClipRegion(event->region());
    QStyleOptionFrame option;
    option.init(this);
    painter.drawPrimitive(QStyle::PE_PanelTipLabel, option);
    painter.end();

    QWidget::paintEvent(event);
}

