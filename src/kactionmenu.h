/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 1999 Reginald Stadlbauer <reggie@kde.org>
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 2000 Nicolas Hadacek <haadcek@kde.org>
    SPDX-FileCopyrightText: 2000 Kurt Granroth <granroth@kde.org>
    SPDX-FileCopyrightText: 2000 Michael Koch <koch@kde.org>
    SPDX-FileCopyrightText: 2001 Holger Freyther <freyther@kde.org>
    SPDX-FileCopyrightText: 2002 Ellis Whitehead <ellis@kde.org>
    SPDX-FileCopyrightText: 2003 Andras Mantia <amantia@kde.org>
    SPDX-FileCopyrightText: 2005-2006 Hamish Rodda <rodda@kde.org>

    SPDX-License-Identifier: LGPL-2.0-only
*/

#ifndef KACTIONMENU_H
#define KACTIONMENU_H

#include <QToolButton>
#include <QWidgetAction>
#include <memory>

#include <kwidgetsaddons_export.h>

class QMenu;

/**
 * @class KActionMenu kactionmenu.h KActionMenu
 *
 * A KActionMenu is an action that provides a sub-menu of other actions.
 *
 * Plugged in a popupmenu, it will create a submenu.
 * Plugged in a toolbar, it will create a button with a popup menu.
 *
 * This is the action used by the XMLGUI since it holds other actions.
 * If you want a submenu for selecting one tool among many (without icons), see KSelectAction.
 */
class KWIDGETSADDONS_EXPORT KActionMenu : public QWidgetAction
{
    Q_OBJECT
#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 77)
    Q_PROPERTY(bool delayed READ delayed WRITE setDelayed)
    Q_PROPERTY(bool stickyMenu READ stickyMenu WRITE setStickyMenu)
#endif
    Q_PROPERTY(QToolButton::ToolButtonPopupMode popupMode READ popupMode WRITE setPopupMode)

public:
    explicit KActionMenu(QObject *parent);
    KActionMenu(const QString &text, QObject *parent);
    KActionMenu(const QIcon &icon, const QString &text, QObject *parent);
    ~KActionMenu() override;

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 0)
    /**
     * @deprecated Since 5.0.
     */
    KWIDGETSADDONS_DEPRECATED_VERSION(5, 0, "Use KActionMenu::removeAction(QAction*)")
    void remove(QAction *);
#endif

    /**
     * Adds @p action to this KActionMenu.
     * The KActionMenu does not take ownership of @p action.
     */
    void addAction(QAction *action);
    QAction *addSeparator();
    void insertAction(QAction *before, QAction *action);
    QAction *insertSeparator(QAction *before);
    void removeAction(QAction *action);

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 0)
    /**
     * Returns this action's menu as a KMenu, if it is one.
     * If none exists, one will be created.
     * @deprecated Since 5.0, use menu() instead.
     */
    KWIDGETSADDONS_DEPRECATED_VERSION(5, 0, "Use KActionMenu::menu()")
    inline QMenu *popupMenu()
    {
        return menu();
    }
#endif

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 77)
    /**
     * Returns true if this action creates a delayed popup menu
     * when plugged in a KToolBar.
     *
     * @deprecated Since 5.77, use popupMode() instead.
     */
    KWIDGETSADDONS_DEPRECATED_VERSION(5, 77, "Use KActionMenu::popupMode()")
    bool delayed() const;
#endif

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 77)
    /**
     * If set to true, this action will create a delayed popup menu
     * when plugged in a KToolBar. Otherwise it creates a normal popup.
     * Default: true
     *
     * Remember that if the "main" action (the toolbar button itself)
     * cannot be clicked, then you should call setDelayed(false).
     *
     * In the other case, if the main action can be clicked, it can only happen
     * in a toolbar: in a menu, the parent of a submenu can't be activated.
     * To get a "normal" menu item when plugged a menu (and no submenu)
     * use KToolBarPopupAction.
     *
     * @deprecated Since 5.77, use setPopupMode() instead.
     */
    KWIDGETSADDONS_DEPRECATED_VERSION(5, 77, "Use KActionMenu::setPopupMode()")
    void setDelayed(bool delayed);
#endif

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 77)
    /**
     * Returns true if this action creates a sticky popup menu.
     * @see setStickyMenu().
     * @deprecated Since 5.77, use popupMode() instead.
     */
    KWIDGETSADDONS_DEPRECATED_VERSION(5, 77, "Use KActionMenu::popupMode()")
    bool stickyMenu() const;
#endif

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 77)
    /**
     * If set to true, this action will create a sticky popup menu
     * when plugged in a KToolBar.
     * "Sticky", means it's visible until a selection is made or the mouse is
     * clicked elsewhere. This feature allows you to make a selection without
     * having to press and hold down the mouse while making a selection.
     * Default: sticky.
     *
     * @deprecated Since 5.77, use setPopupMode() instead.
     */
    KWIDGETSADDONS_DEPRECATED_VERSION(5, 77, "Use KActionMenu::setPopupMode()")
    void setStickyMenu(bool sticky);
#endif

    /**
     * The currently used popup mode when plugged in a KToolBar.
     *
     * @see setPopupMode()
     *
     * @since 5.77
     */
    QToolButton::ToolButtonPopupMode popupMode() const;

    /**
     * Determines the popup mode when plugged in a KToolBar.
     *
     * Options are:
     *  - QToolButton::InstantPopup
     *    Clicking anywhere on the toolbar button opens the popup menu.
     *  - QToolButton::DelayedPopup (Default)
     *    Clicking anywhere on the toolbar button triggers the default action.
     *    Clicking and holding the toolbar button opens the popup menu instead.
     *  - QToolButton::MenuButtonPopup
     *    The toolbar button is split in a main button (triggers default action)
     *    and an arrow button (opens the popup menu).
     *
     * @see QToolButton
     *
     * @since 5.77
     */
    void setPopupMode(QToolButton::ToolButtonPopupMode popupMode);

    QWidget *createWidget(QWidget *parent) override;

private:
    std::unique_ptr<class KActionMenuPrivate> const d;
};

#endif
