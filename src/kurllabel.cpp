// Copyright (C) 2000 Peter Putzer <putzer@kde.org>

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301  USA

#include "kurllabel.h"

#include <QTimer>
#include <QApplication>
#include <QMouseEvent>
#include <QPalette>

class Q_DECL_HIDDEN KUrlLabel::Private
{
public:
    Private(const QString &_url, KUrlLabel *_parent)
        : parent(_parent),
          url(_url),
          tipText(url),
          linkColor(_parent->palette().color(QPalette::Active, QPalette::Link)),
          highlightedLinkColor(_parent->palette().color(QPalette::Active, QPalette::BrightText)),
          cursor(nullptr),
          textUnderlined(true),
          realUnderlined(true),
          useTips(false),
          useCursor(false),
          glowEnabled(true),
          floatEnabled(false),
          timer(new QTimer(parent))
    {
        connect(timer, &QTimer::timeout, parent, [this]() { updateColor(); });
    }

    ~Private()
    {
    }

    void updateColor()
    {
        timer->stop();

        if (!(glowEnabled || floatEnabled) || !parent->rect().contains(parent->mapFromGlobal(QCursor::pos()))) {
            setLinkColor(linkColor);
        }
    }

    void setLinkColor(const QColor &color)
    {
        QPalette palette = parent->palette();
        palette.setColor(QPalette::WindowText, color);
        parent->setPalette(palette);

        parent->update();
    }

    KUrlLabel *parent;

    QString url;
    QString tipText;
    QColor linkColor;
    QColor highlightedLinkColor;
    QCursor *cursor;
    bool textUnderlined : 1;
    bool realUnderlined : 1;
    bool useTips : 1;
    bool useCursor : 1;
    bool glowEnabled : 1;
    bool floatEnabled : 1;
    QPixmap alternatePixmap;
    QPixmap realPixmap;
    QTimer *timer;
};

KUrlLabel::KUrlLabel(const QString &url, const QString &text, QWidget *parent)
    : QLabel(!text.isNull() ? text : url, parent),
      d(new Private(url, this))
{
    setFont(font());
    setCursor(QCursor(Qt::PointingHandCursor));
    d->setLinkColor(d->linkColor);
}

KUrlLabel::KUrlLabel(QWidget *parent)
    : QLabel(parent),
      d(new Private(QString(), this))
{
    setFont(font());
    setCursor(QCursor(Qt::PointingHandCursor));
    d->setLinkColor(d->linkColor);
}

KUrlLabel::~KUrlLabel()
{
    delete d;
}

void KUrlLabel::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);

    d->setLinkColor(d->highlightedLinkColor);
    d->timer->start(300);

    switch (event->button()) {
    case Qt::LeftButton:
        emit leftClickedUrl();
#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 65)
        emit leftClickedUrl(d->url);
#endif
        break;

    case Qt::MidButton:
        emit middleClickedUrl();
#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 65)
        emit middleClickedUrl(d->url);
#endif
        break;

    case Qt::RightButton:
        emit rightClickedUrl();
#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 65)
        emit rightClickedUrl(d->url);
#endif
        break;

    default:
        break;
    }
}

void KUrlLabel::setFont(const QFont &font)
{
    QFont newFont = font;
    newFont.setUnderline(d->textUnderlined);

    QLabel::setFont(newFont);
}

void KUrlLabel::setUnderline(bool on)
{
    d->textUnderlined = on;

    setFont(font());
}

void KUrlLabel::setUrl(const QString &url)
{
    if (d->tipText == d->url) {   // update the tip as well
        d->tipText = url;
        setUseTips(d->useTips);
    }

    d->url = url;
}

QString KUrlLabel::url() const
{
    return d->url;
}

void KUrlLabel::setUseCursor(bool on, QCursor *cursor)
{
    d->useCursor = on;
    d->cursor = cursor;

    if (on) {
        if (cursor) {
            setCursor(*cursor);
        } else {
            setCursor(QCursor(Qt::PointingHandCursor));
        }
    } else {
        unsetCursor();
    }
}

bool KUrlLabel::useCursor() const
{
    return d->useCursor;
}

void KUrlLabel::setUseTips(bool on)
{
    d->useTips = on;

    if (on) {
        setToolTip(d->tipText);
    } else {
        setToolTip(QString());
    }
}

void KUrlLabel::setTipText(const QString &tipText)
{
    d->tipText = tipText;

    setUseTips(d->useTips);
}

bool KUrlLabel::useTips() const
{
    return d->useTips;
}

QString KUrlLabel::tipText() const
{
    return d->tipText;
}

void KUrlLabel::setHighlightedColor(const QColor &color)
{
    d->linkColor = color;

    if (!d->timer->isActive()) {
        d->setLinkColor(color);
    }
}

void KUrlLabel::setHighlightedColor(const QString &color)
{
    setHighlightedColor(QColor(color));
}

void KUrlLabel::setSelectedColor(const QColor &color)
{
    d->highlightedLinkColor = color;

    if (d->timer->isActive()) {
        d->setLinkColor(color);
    }
}

void KUrlLabel::setSelectedColor(const QString &color)
{
    setSelectedColor(QColor(color));
}

void KUrlLabel::setGlowEnabled(bool glowEnabled)
{
    d->glowEnabled = glowEnabled;
}

void KUrlLabel::setFloatEnabled(bool floatEnabled)
{
    d->floatEnabled = floatEnabled;
}

bool KUrlLabel::isGlowEnabled() const
{
    return d->glowEnabled;
}

bool KUrlLabel::isFloatEnabled() const
{
    return d->floatEnabled;
}

void KUrlLabel::setAlternatePixmap(const QPixmap &pixmap)
{
    d->alternatePixmap = pixmap;
}

const QPixmap *KUrlLabel::alternatePixmap() const
{
    return &d->alternatePixmap;
}

void KUrlLabel::enterEvent(QEvent *event)
{
    QLabel::enterEvent(event);

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    if (!d->alternatePixmap.isNull()) {
        const auto currentPixmap = pixmap(Qt::ReturnByValue);
        if (!currentPixmap.isNull()) {
            d->realPixmap = currentPixmap;
            setPixmap(d->alternatePixmap);
        }
    }
#else
    if (!d->alternatePixmap.isNull() && pixmap()) {
        d->realPixmap = *pixmap();
        setPixmap(d->alternatePixmap);
    }
#endif

    if (d->glowEnabled || d->floatEnabled) {
        d->timer->stop();

        d->setLinkColor(d->highlightedLinkColor);

        d->realUnderlined = d->textUnderlined;

        if (d->floatEnabled) {
            setUnderline(true);
        }
    }
    emit enteredUrl();
#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 65)
    emit enteredUrl(d->url);
#endif
}

void KUrlLabel::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);

    if (!d->alternatePixmap.isNull() &&
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        !pixmap(Qt::ReturnByValue).isNull()) {
#else
        pixmap()) {
#endif
        setPixmap(d->realPixmap);
    }

    if ((d->glowEnabled || d->floatEnabled) && !d->timer->isActive()) {
        d->setLinkColor(d->linkColor);
    }

    setUnderline(d->realUnderlined);
    emit leftUrl();
#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 65)
    emit leftUrl(d->url);
#endif
}

bool KUrlLabel::event(QEvent *event)
{
    if (event->type() == QEvent::PaletteChange) {
        /**
         * Use parentWidget() unless you are a toplevel widget, then try qAapp
         */
        QPalette palette = parentWidget() ? parentWidget()->palette() : qApp->palette();

        palette.setBrush(QPalette::Base, palette.brush(QPalette::Normal, QPalette::Window));
        palette.setColor(QPalette::WindowText, this->palette().color(QPalette::Active, QPalette::WindowText));
        setPalette(palette);

        d->linkColor = palette.color(QPalette::Active, QPalette::Link);
        d->updateColor();

        return true;
    } else {
        return QLabel::event(event);
    }
}

#include "moc_kurllabel.cpp"
