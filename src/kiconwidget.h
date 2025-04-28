/*
 * SPDX-FileCopyrightText: 2026 Kai Uwe Broulik <kde@broulik.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KICONWIDGET_H
#define KICONWIDGET_H

#include <kwidgetsaddons_export.h>

#include <QIcon>
#include <QWidget>

#include <memory>

/*!
 * \class KIconWidget kiconwidget.h KIconWidget
 *
 * \brief A widget that displays a QIcon
 *
 * QLabel can only display a fixed pixmap. In order to facilitate
 * high-dpi and mixed-dpi screens, this widget takes a QIcon and
 * renders it using the appropriate devicePixelRatio, automatically
 * updating as needed. It also updates when the palette changes.
 *
 * \since 6.32
 */
class KWIDGETSADDONS_EXPORT KIconWidget : public QWidget
{
    Q_OBJECT
    /*!
     * \property KIconWidget::icon
     */
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon USER true)
    /*!
     * \property KIconWidget::iconSize
     */
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize RESET resetIconSize)
    /*!
     * \property KIconWidget::mode
     */
    Q_PROPERTY(QIcon::Mode mode READ mode WRITE setMode)
    /*!
     * \property KIconWidget::state
     */
    Q_PROPERTY(QIcon::State state READ state WRITE setState)

public:
    /*!
     * Constructor.
     *
     * \a parent the label's parent widget
     */
    explicit KIconWidget(QWidget *parent = nullptr);

    /*!
     * Constructor.
     *
     * \a icon the icon that will be displayed
     * \a parent the label's parent widget
     */
    explicit KIconWidget(const QIcon &icon, QWidget *parent = nullptr);

    /*!
     * Constructor.
     *
     * \a icon the icon that will be displayed
     * \a iconSize the size at which the icon will be displayed
     * \a parent the label's parent widget
     */
    explicit KIconWidget(const QIcon &icon, QSize iconSize, QWidget *parent = nullptr);

    ~KIconWidget() override;

    /*!
     * Returns the icon that is being displayed.
     */
    QIcon icon() const;

    /*!
     * Set the icon to display
     *
     * \a icon The icon to display
     */
    void setIcon(const QIcon &icon);

    /*!
     * Returns the local size the icon should be displayed at.
     */
    QSize iconSize() const;

    /*!
     * Set the logical icon size.
     *
     * The default is style-dependent PM_LargeIconSize.
     *
     * \a iconSize The logical size the icon should be displayed at.
     */
    void setIconExtent(int iconSize);

    /*!
     * Set the logical icon size.
     *
     * The default is style-dependent PM_LargeIconSize.
     *
     * \a iconSize The logical size the icon should be displayed at.
     */
    void setIconSize(QSize iconSize);

    /*!
     * Reset icon size to the default PM_LargeIconSize.
     */
    void resetIconSize();

    /*!
     * Returns the icon mode.
     */
    QIcon::Mode mode() const;

    /*!
     * Set the icon mode.
     *
     * The default is QIcon::Normal.
     *
     * \a mode The icon mode.
     */
    void setMode(QIcon::Mode mode);

    /*!
     * Returns the icon state.
     */
    QIcon::State state() const;

    /*!
     * Set the icon state.
     *
     * The default is QIcon::Off.
     *
     * \a state The icon state.
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
