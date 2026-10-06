/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2008 Rafael Fernández López <ereslibre@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kcapacitybar.h"
#include "kstyleextensions.h"

#include <math.h>

#include <QLinearGradient>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionProgressBar>

#define ROUND_MARGIN 6
#define VERTICAL_SPACING 1

class KCapacityBarPrivate
{
public:
    KCapacityBarPrivate(KCapacityBar::DrawTextMode drawTextMode)
        : drawTextMode(drawTextMode)
    {
    }

    QString text;
    int value = 0;
    bool fillFullBlocks = true;
    bool continuous = true;
    int barHeight = 12;
    Qt::Alignment horizontalTextAlignment = Qt::AlignCenter;
    QStyle::ControlElement ce_capacityBar = QStyle::ControlElement(0);

    KCapacityBar::DrawTextMode drawTextMode;
};

KCapacityBar::KCapacityBar(QWidget *parent)
    : KCapacityBar(DrawTextOutline, parent)
{
}

KCapacityBar::KCapacityBar(KCapacityBar::DrawTextMode drawTextMode, QWidget *parent)
    : QWidget(parent)
    , d(new KCapacityBarPrivate(drawTextMode))
{
    d->ce_capacityBar = KStyleExtensions::customControlElement(QStringLiteral("CE_CapacityBar"), this);
}

KCapacityBar::~KCapacityBar() = default;

void KCapacityBar::setValue(int value)
{
    d->value = value;
    update();
}

int KCapacityBar::value() const
{
    return d->value;
}

void KCapacityBar::setText(const QString &text)
{
    bool updateGeom = d->text.isEmpty() || text.isEmpty();
    d->text = text;
    if (updateGeom) {
        updateGeometry();
    }

#ifndef QT_NO_ACCESSIBILITY
    setAccessibleName(text);
#endif

    update();
}

QString KCapacityBar::text() const
{
    return d->text;
}

void KCapacityBar::setFillFullBlocks(bool fillFullBlocks)
{
    d->fillFullBlocks = fillFullBlocks;
    update();
}

bool KCapacityBar::fillFullBlocks() const
{
    return d->fillFullBlocks;
}

void KCapacityBar::setContinuous(bool continuous)
{
    d->continuous = continuous;
    update();
}

bool KCapacityBar::continuous() const
{
    return d->continuous;
}

void KCapacityBar::setBarHeight(int barHeight)
{
    // automatically convert odd values to even. This will make the bar look
    // better.
    d->barHeight = (barHeight % 2) ? barHeight + 1 : barHeight;
    updateGeometry();
}

int KCapacityBar::barHeight() const
{
    return d->barHeight;
}

void KCapacityBar::setHorizontalTextAlignment(Qt::Alignment horizontalTextAlignment)
{
    Qt::Alignment alignment = horizontalTextAlignment;

    // if the value came with any vertical alignment flag, remove it.
    alignment &= ~Qt::AlignTop;
    alignment &= ~Qt::AlignBottom;
    alignment &= ~Qt::AlignVCenter;

    d->horizontalTextAlignment = alignment;
    update();
}

Qt::Alignment KCapacityBar::horizontalTextAlignment() const
{
    return d->horizontalTextAlignment;
}

void KCapacityBar::setDrawTextMode(DrawTextMode mode)
{
    d->drawTextMode = mode;
    update();
}

KCapacityBar::DrawTextMode KCapacityBar::drawTextMode() const
{
    return d->drawTextMode;
}

void KCapacityBar::drawCapacityBar(QPainter *p, const QRect &rect) const
{
    drawCapacityBar(p, rect, {});
}

void KCapacityBar::drawCapacityBar(QPainter *p, const QRect &rect, QStyle::State state) const
{
    if (d->ce_capacityBar) {
        QStyleOptionProgressBar opt;
        opt.initFrom(this);
        opt.rect = rect;
        opt.minimum = 0;
        opt.maximum = 100;
        opt.progress = d->value;
        opt.state |= (state | QStyle::State_Horizontal);
        opt.text = d->text;
        opt.textAlignment = Qt::AlignCenter;
        opt.textVisible = !d->text.isEmpty();
        style()->drawControl(d->ce_capacityBar, &opt, p, this);

        return;
    }
    p->save();
    p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QRect drawRect(rect);
    if (d->drawTextMode == DrawTextOutline) {
        drawRect.setHeight(d->barHeight);
    }

    QStyleOptionProgressBar opt;
    opt.initFrom(this);
    opt.rect = drawRect;
    opt.minimum = 0;
    opt.maximum = 100;
    opt.progress = d->value;
    opt.state |= (state | QStyle::State_Horizontal);
    opt.text = d->text;
    opt.textAlignment = Qt::AlignCenter;
    opt.textVisible = d->drawTextMode == DrawTextInline;
    style()->drawControl(QStyle::CE_ProgressBar, &opt, p, this);

    p->restore();
    if (d->drawTextMode == DrawTextOutline) {
        drawRect.setWidth(drawRect.width() - 4);
        drawRect.setHeight(drawRect.height() - 2);
        p->drawText(rect, Qt::AlignBottom | d->horizontalTextAlignment, fontMetrics().elidedText(d->text, Qt::ElideRight, drawRect.width()));
    }
}

QSize KCapacityBar::minimumSizeHint() const
{
    int width = fontMetrics().boundingRect(d->text).width() + ((d->drawTextMode == KCapacityBar::DrawTextInline) ? ROUND_MARGIN * 2 : 0);

    int height = (d->drawTextMode == KCapacityBar::DrawTextInline) ? qMax(fontMetrics().height(), d->barHeight)
                                                                   : (d->text.isEmpty() ? 0 : fontMetrics().height() + VERTICAL_SPACING * 2) + d->barHeight;

    if (height % 2) {
        height++;
    }

    return QSize(width, height);
}

void KCapacityBar::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setClipRect(event->rect());
    drawCapacityBar(&p, contentsRect());
    p.end();
}

void KCapacityBar::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    if (event->type() == QEvent::StyleChange) {
        d->ce_capacityBar = KStyleExtensions::customControlElement(QStringLiteral("CE_CapacityBar"), this);
    }
}

#include "moc_kcapacitybar.cpp"
