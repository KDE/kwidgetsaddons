/*
 * SPDX-FileCopyrightText: 2026 Kai Uwe Broulik <kde@broulik.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "kiconwidget.h"

#include <QEvent>
#include <QPainter>
#include <QPixmap>
#include <QStyle>
#include <QStyleOption>

class KIconWidgetPrivate
{
public:
    void invalidatePixmap();

    QIcon icon;
    QSize iconSize;
    QIcon::Mode mode = QIcon::Normal;
    QIcon::State state = QIcon::Off;

    QPixmap pixmap;
};

void KIconWidgetPrivate::invalidatePixmap()
{
    pixmap = QPixmap{};
}

KIconWidget::KIconWidget(const QIcon &icon, QSize iconSize, QWidget *parent)
    : QWidget(parent)
    , d(std::make_unique<KIconWidgetPrivate>())
{
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed, QSizePolicy::Label));
    d->icon = icon;
    d->iconSize = iconSize;
}

KIconWidget::KIconWidget(const QIcon &icon, QWidget *parent)
    : KIconWidget(icon, QSize(), parent)
{
}

KIconWidget::KIconWidget(QWidget *parent)
    : KIconWidget(QIcon(), parent)
{
}

KIconWidget::~KIconWidget() = default;

QIcon KIconWidget::icon() const
{
    return d->icon;
}

void KIconWidget::setIcon(const QIcon &icon)
{
    d->icon = icon;

    d->invalidatePixmap();
    updateGeometry();
    update();
}

QSize KIconWidget::iconSize() const
{
    if (!d->iconSize.isEmpty()) {
        return d->iconSize;
    }

    QStyleOption option;
    option.initFrom(this);
    const int largeIconSize = style()->pixelMetric(QStyle::PM_LargeIconSize, &option, this);
    return QSize(largeIconSize, largeIconSize);
}

void KIconWidget::setIconExtent(int iconSize)
{
    setIconSize(QSize(iconSize, iconSize));
}

void KIconWidget::setIconSize(QSize iconSize)
{
    if (d->iconSize == iconSize) {
        return;
    }

    d->iconSize = iconSize;
    d->invalidatePixmap();
    updateGeometry();
    update();
}

void KIconWidget::resetIconSize()
{
    setIconSize(QSize());
}

QIcon::Mode KIconWidget::mode() const
{
    return d->mode;
}

void KIconWidget::setMode(QIcon::Mode mode)
{
    if (d->mode == mode) {
        return;
    }

    d->mode = mode;
    d->invalidatePixmap();
    update();
}

QIcon::State KIconWidget::state() const
{
    return d->state;
}

void KIconWidget::setState(QIcon::State state)
{
    if (d->state == state) {
        return;
    }

    d->state = state;
    d->invalidatePixmap();
    update();
}

QSize KIconWidget::sizeHint() const
{
    return !d->icon.isNull() ? iconSize() : QSize();
}

QSize KIconWidget::minimumSizeHint() const
{
    return !d->icon.isNull() ? iconSize() : QSize();
}

bool KIconWidget::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::ApplicationPaletteChange:
    case QEvent::PaletteChange:
        d->invalidatePixmap();
        break;
    case QEvent::StyleChange:
        // PM_LargeIconSize might have changed.
        if (d->iconSize.isEmpty()) {
            d->invalidatePixmap();
            updateGeometry();
        }
        break;
    default:
        break;
    }

    return QWidget::event(event);
}

void KIconWidget::paintEvent(QPaintEvent *event)
{
    // TODO skip painting/creating icon when repaint region is outside iconRect?
    Q_UNUSED(event)

    if (d->icon.isNull()) {
        return;
    }

    if (d->pixmap.isNull() || d->pixmap.devicePixelRatioF() != devicePixelRatioF()) {
        d->pixmap = d->icon.pixmap(iconSize(), devicePixelRatioF(), d->mode, d->state);
    }

    // Pixmap creation can still fail even if the icon is valid.
    if (d->pixmap.isNull()) {
        return;
    }

    QRectF iconRect{QPointF{0, 0}, d->pixmap.deviceIndependentSize()};
    // iconRect.moveCenter(rect().center()) does an off-by-one.
    const QPointF newPosition{(rect().width() - iconRect.width()) / 2.0, (rect().height() - iconRect.height()) / 2.0};
    iconRect.moveTopLeft(newPosition);

    QPainter painter(this);
    painter.drawPixmap(iconRect.toRect(), d->pixmap);
}

#include "moc_kiconwidget.cpp"
