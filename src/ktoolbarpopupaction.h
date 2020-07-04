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

#ifndef KTOOLBARPOPUPACTION_H
#define KTOOLBARPOPUPACTION_H

#include <QWidgetAction>

#include <kwidgetsaddons_export.h>

class QMenu;

/**
 * @class KToolBarPopupAction ktoolbarpopupaction.h KToolBarPopupAction
 *
 * This action is a normal action everywhere, except in a toolbar
 * where it also has a popupmenu (optionally delayed). This action is designed
 * for history actions (back/forward, undo/redo) and for any other action
 * that has more detail in a toolbar than in a menu (e.g. tool chooser
 * with "Other" leading to a dialog...).
 *
 * In contrast to KActionMenu, this action is a \e simple menuitem when plugged
 * into a menu, and has a popup only in a toolbar.
 *
 * Use cases include Back/Forward, and Undo/Redo. Simple click is what's most commonly
 * used, and enough for menus, but in toolbars there is \e also an optional popup
 * to go back N steps or undo N steps.
 */
class KWIDGETSADDONS_EXPORT KToolBarPopupAction : public QWidgetAction
{
    Q_OBJECT
    Q_PROPERTY(bool delayed READ delayed WRITE setDelayed)
    Q_PROPERTY(bool stickyMenu READ stickyMenu WRITE setStickyMenu)

public:
    //Not all constructors - because we need an icon, since this action only makes
    // sense when being plugged at least in a toolbar.
    /**
     * Create a KToolBarPopupAction, with a text, an icon, a
     * parent and a name.
     *
     * @param icon The icon to display.
     * @param text The text that will be displayed.
     * @param parent This action's parent.
     */
    KToolBarPopupAction(const QIcon &icon, const QString &text, QObject *parent);

    /**
     * Destroys the toolbar popup action.
     */
    ~KToolBarPopupAction() override;

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 0)
    /**
     * The popup menu that is shown when clicking (some time) on the toolbar
     * button. You may want to plug items into it on creation, or connect to
     * aboutToShow for a more dynamic menu.
     *
     * \deprecated Since 5.0, use menu() instead
     */
    KWIDGETSADDONS_DEPRECATED_VERSION(5, 0, "Use KToolBarPopupAction::menu()")
    QMenu *popupMenu() const;
#endif

    /**
     * Returns true if this action creates a delayed popup menu
     * when plugged in a KToolBar.
     */
    bool delayed() const;

    /**
     * If set to true, this action will create a delayed popup menu
     * when plugged in a KToolBar. Otherwise it creates a normal popup.
     * Default: delayed.
     */
    void setDelayed(bool delayed);

    /**
     * Returns true if this action creates a sticky popup menu.
     * @see setStickyMenu().
     */
    bool stickyMenu() const;

    /**
     * If set to true, this action will create a sticky popup menu
     * when plugged in a KToolBar.
     * "Sticky", means it's visible until a selection is made or the mouse is
     * clicked elsewhere. This feature allows you to make a selection without
     * having to press and hold down the mouse while making a selection.
     * Only available if delayed() is true.
     * Default: sticky.
     */
    void setStickyMenu(bool sticky);

    /**
     * Reimplemented from QWidgetAction.
     */
    QWidget *createWidget(QWidget *parent) override;

private:
    class Private;
    Private *const d;
};

#endif
