/*
 * SPDX-FileCopyrightText: 2025 Kai Uwe Broulik <kde@broulik.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KICONWIDGET_H
#define KICONWIDGET_H

#include <kwidgetsaddons_export.h>

#include <QIcon>
#include <QWidget>

#include <memory>

/**
 * @class KIconWidget kiconwidget.h KIconWidget
 *
 * @short A widget that displays a QIcon
 *
 * QLabel can only display a fixed pixmap. In order to facilitate
 * high-dpi and mixed-dpi screens, this widget takes a QIcon and
 * renders it using the appropriate devicePixelRatio, automatically
 * updating as needed. It also updates when the palette changes.
 *
 * @since 6.19
 */
class KWIDGETSADDONS_EXPORT KIconWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize RESET resetIconSize)
    Q_PROPERTY(QIcon::Mode mode READ mode WRITE setMode)
    Q_PROPERTY(QIcon::State state READ state WRITE setState)

public:
    /**
     * Default constructor.
     * @param parent the label's parent object
     */
    explicit KIconWidget(QWidget *parent = nullptr);

    /**
     * @param icon the icon that will be displayed
     * @param parent the label's parent object
     */
    explicit KIconWidget(const QIcon &icon, QWidget *parent = nullptr);

    /**
     * @param icon the icon that will be displayed
     * @param iconSize the size at which the icon will be displayed
     * @param parent the label's parent object
     */
    explicit KIconWidget(const QIcon &icon, const QSize &iconSize, QWidget *parent = nullptr);

    /**
     * Destroys the icon widget.
     */
    ~KIconWidget() override;

    /**
     * @return The icon that is being displayed.
     */
    QIcon icon() const;

    /**
     * @param icon The icon to display
     */
    void setIcon(const QIcon &icon);

    /**
     * @return The logical size the icon should be displayed at.
     */
    QSize iconSize() const;

    /**
     * The default is style-dependent PM_LargeIconSize.
     *
     * @param iconSize The logical size the icon should be displayed at.
     */
    void setIconSize(int iconSize);

    /**
     * The default is style-dependent PM_LargeIconSize.
     *
     * @param iconSize The logical size the icon should be displayed at.
     */
    void setIconSize(const QSize &iconSize);

    /**
     * Reset icon size to the default PM_LargeIconSize.
     */
    void resetIconSize();

    /**
     * @return The icon mode
     */
    QIcon::Mode mode() const;

    /**
     * The default is QIcon::Normal.
     *
     * @param mode The icon mode.
     */
    void setMode(QIcon::Mode mode);

    /**
     * @return The icon state
     */
    QIcon::State state() const;

    /**
     * The default is QIcon::Off.

     * @param state The icon state.
     */
    void setState(QIcon::State state);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    bool event(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    std::unique_ptr<class KIconWidgetPrivate> const d;
};

#endif // KICONWIDGET_H
