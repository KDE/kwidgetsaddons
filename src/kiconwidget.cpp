/*
 * SPDX-FileCopyrightText: 2025 Kai Uwe Broulik <kde@broulik.de>
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
    explicit KIconWidgetPrivate(KIconWidget *q);

    QSize effectiveIconSize() const;
    void invalidatePixmap();

    KIconWidget *const q;
    QIcon icon;
    QSize iconSize;
    QIcon::Mode mode = QIcon::Normal;
    QIcon::State state = QIcon::Off;

    QPixmap pixmap;
};

KIconWidgetPrivate::KIconWidgetPrivate(KIconWidget *q)
    : q(q)
{
}

QSize KIconWidgetPrivate::effectiveIconSize() const
{
    if (!iconSize.isEmpty()) {
        return iconSize;
    } else {
        QStyleOption option;
        option.initFrom(q);
        const int largeIconSize = q->style()->pixelMetric(QStyle::PM_LargeIconSize, &option, q);
        return QSize(largeIconSize, largeIconSize);
    }
}

void KIconWidgetPrivate::invalidatePixmap()
{
    pixmap = QPixmap{};
}

KIconWidget::KIconWidget(const QIcon &icon, const QSize &iconSize, QWidget *parent)
    : QWidget(parent)
    , d(std::make_unique<KIconWidgetPrivate>(this))
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
    return d->effectiveIconSize();
}

void KIconWidget::setIconSize(int iconSize)
{
    setIconSize(QSize(iconSize, iconSize));
}

void KIconWidget::setIconSize(const QSize &iconSize)
{
    if (d->iconSize == iconSize) {
        return;
    }

    d->iconSize = iconSize;
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
    return !d->icon.isNull() ? d->effectiveIconSize() : QSize();
}

QSize KIconWidget::minimumSizeHint() const
{
    return !d->icon.isNull() ? d->effectiveIconSize() : QSize();
}

bool KIconWidget::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::ApplicationPaletteChange:
    case QEvent::PaletteChange:
        d->invalidatePixmap();
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

    const QSize size = d->effectiveIconSize();

    QRect iconRect = QRect(QPoint(0, 0), size);
    iconRect.moveCenter(rect().center());

    if (d->pixmap.isNull() || d->pixmap.deviceIndependentSize() != size || d->pixmap.devicePixelRatioF() != devicePixelRatioF()) {
        d->pixmap = d->icon.pixmap(size, devicePixelRatioF(), d->mode, d->state);
    }

    QPainter painter(this);
    painter.drawPixmap(iconRect, d->pixmap);
}

#include "moc_kiconwidget.cpp"
