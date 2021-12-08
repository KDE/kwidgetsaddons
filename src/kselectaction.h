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
    SPDX-FileCopyrightText: 2006 Albert Astals Cid <aacid@kde.org>
    SPDX-FileCopyrightText: 2006 Clarence Dang <dang@kde.org>
    SPDX-FileCopyrightText: 2006 Michel Hermier <michel.hermier@gmail.com>
    SPDX-FileCopyrightText: 2007 Nick Shaforostoff <shafff@ukr.net>

    SPDX-License-Identifier: LGPL-2.0-only
*/

#ifndef KSELECTACTION_H
#define KSELECTACTION_H

#include <QToolButton>
#include <QWidgetAction>
#include <memory>

#include <kwidgetsaddons_export.h>

class KSelectActionPrivate;

/**
 *  @class KSelectAction kselectaction.h KSelectAction
 *
 *  @short Action for selecting one of several items
 *
 *  Action for selecting one of several items.
 *
 *  This action shows up a submenu with a list of items.
 *  One of them can be checked. If the user clicks on an item
 *  this item will automatically be checked,
 *  the formerly checked item becomes unchecked.
 *  There can be only one item checked at a time.
 */
class KWIDGETSADDONS_EXPORT KSelectAction : public QWidgetAction
{
    Q_OBJECT
    Q_PROPERTY(QAction *currentAction READ currentAction WRITE setCurrentAction)
    Q_PROPERTY(bool editable READ isEditable WRITE setEditable)
    Q_PROPERTY(int comboWidth READ comboWidth WRITE setComboWidth)
    Q_PROPERTY(QString currentText READ currentText)
    Q_PROPERTY(ToolBarMode toolBarMode READ toolBarMode WRITE setToolBarMode)
    Q_PROPERTY(QToolButton::ToolButtonPopupMode toolButtonPopupMode READ toolButtonPopupMode WRITE setToolButtonPopupMode)
    Q_PROPERTY(int currentItem READ currentItem WRITE setCurrentItem)
    Q_PROPERTY(QStringList items READ items WRITE setItems)

public:
    /**
     * Constructs a selection action with the specified parent.
     *
     * @param parent The action's parent object.
     */
    explicit KSelectAction(QObject *parent);

    /**
     * Constructs a selection action with text; a shortcut may be specified by
     * the ampersand character (e.g.\ "&Option" creates a shortcut with key \e O )
     *
     * This is the most common KSelectAction used when you do not have a
     * corresponding icon (note that it won't appear in the current version
     * of the "Edit ToolBar" dialog, because an action needs an icon to be
     * plugged in a toolbar...).
     *
     * @param text The text that will be displayed.
     * @param parent The action's parent object.
     */
    KSelectAction(const QString &text, QObject *parent);

    /**
     * Constructs a selection action with text and an icon; a shortcut may be specified by
     * the ampersand character (e.g.\ "&Option" creates a shortcut with key \e O )
     *
     * This is the other common KSelectAction used.  Use it when you
     * \e do have a corresponding icon.
     *
     * @param icon The icon to display.
     * @param text The text that will be displayed.
     * @param parent The action's parent object.
     */
    KSelectAction(const QIcon &icon, const QString &text, QObject *parent);

    /**
     * Destructor
     */
    ~KSelectAction() override;

    enum ToolBarMode {
        /// Creates a button which pops up a menu when interacted with, as defined by toolButtonPopupMode().
        MenuMode,
        /// Creates a combo box which contains the actions.
        /// This is the default.
        ComboBoxMode,
    };
    Q_ENUM(ToolBarMode)

    /**
     * Returns which type of widget (combo box or button with drop-down menu) will be inserted
     * in a toolbar.
     */
    ToolBarMode toolBarMode() const;

    /**
     * Set the type of widget to be inserted in a toolbar to \a mode.
     */
    void setToolBarMode(ToolBarMode mode);

    /**
     * Returns the style for the list of actions, when this action is plugged
     * into a KToolBar. The default value is QToolButton::InstantPopup
     *
     * \sa QToolButton::setPopupMode()
     */
    QToolButton::ToolButtonPopupMode toolButtonPopupMode() const;

    /**
     * Set how this list of actions should behave when in popup mode and plugged into a toolbar.
     */
    void setToolButtonPopupMode(QToolButton::ToolButtonPopupMode mode);

    /**
     * The action group used to create exclusivity between the actions associated with this action.
     */
    QActionGroup *selectableActionGroup() const;

    /**
     * Returns the current QAction.
     * @see setCurrentAction
     */
    QAction *currentAction() const;

    /**
     * Returns the index of the current item.
     *
     * @sa currentText(), currentAction()
     */
    int currentItem() const;

    /**
     * Returns the text of the currently selected item.
     *
     * @sa currentItem(), currentAction()
     */
    QString currentText() const;

    /**
     * Returns the list of selectable actions
     */
    QList<QAction *> actions() const;

    /**
     * Returns the action at \a index, if one exists.
     */
    QAction *action(int index) const;

    /**
     * Searches for an action with the specified \a text, using a search whose
     * case sensitivity is defined by \a cs.
     */
    QAction *action(const QString &text, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

    /**
     * Sets the currently checked item.
     *
     * @param action the QAction to become the currently checked item.
     *
     * \return \e true if a corresponding action was found and successfully checked.
     */
    bool setCurrentAction(QAction *action);

    /**
     * Convenience function to set the currently checked action to be the action
     * at index \p index.
     *
     * If there is no action at that index, the currently checked action (if any) will
     * be deselected.
     *
     * \return \e true if a corresponding action was found and thus set to the current action, otherwise \e false
     */
    bool setCurrentItem(int index);

    /**
     * Overloaded member function, provided for convenience, to set the currently
     * checked action to be the action which has \p text as its text().
     *
     * If there is no action at that index, the currently checked action (if any) will
     * be deselected.
     *
     * \return \e true if a corresponding action was found, otherwise \e false
     */
    bool setCurrentAction(const QString &text, Qt::CaseSensitivity cs = Qt::CaseSensitive);

    /**
     * Add \a action to the list of selectable actions.
     */
    void addAction(QAction *action);

    /**
     * Overloaded member function, provided for convenience, which creates an action
     * from \p text and inserts it into the list of selectable actions.
     *
     * The newly created action is checkable and not user configurable.
     */
    QAction *addAction(const QString &text);

    /**
     * Overloaded member function, provided for convenience, which creates an action
     * from \p text and \p icon and inserts it into the list of selectable actions.
     *
     * The newly created action is checkable and not user configurable.
     */
    QAction *addAction(const QIcon &icon, const QString &text);

    /**
     * Remove the specified \a action from this action selector.
     *
     * You take ownership here, so save or delete it in order to not leak the action.
     */
    virtual QAction *removeAction(QAction *action);

    /**
     * Inserts the action action to this widget's list of actions, before the action before.
     * It appends the action if before is a null pointer or before is not a valid action for this widget.
     *
     * @since 5.0
     */
    virtual void insertAction(QAction *before, QAction *action);

    /**
     * Convenience function to create the list of selectable items.
     * Any previously existing items will be cleared.
     */
    void setItems(const QStringList &lst);

    /**
     * Convenience function which returns the items that can be selected with this action.
     * It is the same as iterating selectableActionGroup()->actions() and looking at each
     * action's text().
     */
    QStringList items() const;

    /**
     * When this action is plugged into a toolbar, it creates a combobox.
     * @return true if the combo editable.
     */
    bool isEditable() const;

    /**
     * When this action is plugged into a toolbar, it creates a combobox.
     * This makes the combo editable or read-only.
     */
    void setEditable(bool);

    /**
     * When this action is plugged into a toolbar, it creates a combobox.
     * This returns the maximum width set by setComboWidth
     */
    int comboWidth() const;

    /**
     * When this action is plugged into a toolbar, it creates a combobox.
     * This gives a _maximum_ size to the combobox.
     * The minimum size is automatically given by the contents (the items).
     */
    void setComboWidth(int width);

    /**
     * Sets the maximum items that are visible at once if the action
     * is a combobox, that is the number of items in the combobox's viewport
     */
    void setMaxComboViewCount(int n);

    /**
     * Remove and delete all the items in this action.
     *
     * @see removeAllActions()
     */
    void clear();

    /**
     * Remove all the items in this action.
     *
     * Unlike clear(), this will not delete the actions.
     *
     * @see clear()
     */
    void removeAllActions();

    /**
     * Sets whether any occurrence of the ampersand character ( & ) in items
     * should be interpreted as keyboard accelerator for items displayed in a
     * menu or not.  Only applies to (overloaded) methods dealing with QStrings,
     * not those dealing with QActions.
     *
     * Defaults to true.
     *
     * \param b true if ampersands indicate a keyboard accelerator, otherwise false.
     */
    void setMenuAccelsEnabled(bool b);

    /**
     * Returns whether ampersands passed to methods using QStrings are interpreted
     * as keyboard accelerator indicators or as literal ampersands.
     */
    bool menuAccelsEnabled() const;

    /**
     * You should delete KSelectAction::menu() before calling setMenu(). KSelectAction
     * will take the @param menu ownership and it will be deleted when KSelectAction is
     * destroyed.
     */
    using QWidgetAction::setMenu;

    /**
     * Changes the text of item @param index to @param text .
     */
    void changeItem(int index, const QString &text);

Q_SIGNALS:
    /**
     * This signal is emitted when an item is selected.
     * @param action indicates the item selected
     */
    void triggered(QAction *action); // clazy:exclude=overloaded-signal
    // TODO KF6:: rename to actionTriggered(QAction *action)
    // We cannot do this in KF5, due to existing slot method with same signature, see below

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 78)
    /**
     * This signal is emitted when an item is selected.
     * @param index indicates the item selected
     * @deprecated Since 5.78, use indexTriggered(int)
     */
    KWIDGETSADDONS_DEPRECATED_VERSION(5, 78, "Use KSelectAction::indexTriggered(int)")
    void triggered(int index); // clazy:exclude=overloaded-signal
#endif
    /**
     * This signal is emitted when an item is selected.
     * @param index indicates the item selected
     *
     * In your KSelectAction subclass to be backward-compatible to KF < 5.78, emit instead
     * just the deprecated signal triggered(int). That will also
     * automatically emit indexTriggered(int) because this signal
     * is connected to the deprecated one in the KSelectAction constructor.
     * Only if you compile against a variant of KWidgetsAddons built without all API
     * deprecated up to version 5.78, then emit this signal directly.
     * Your code would be like this:
     * @code
     *     const int index = selectableActionGroup()->actions().indexOf(action);
     * #if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 78)
     *     // will also indirectly emit indexTriggered since 5.78
     *     Q_EMIT triggered(index);
     * #else
     *     Q_EMIT indexTriggered(index);
     * #endif
     * @endcode
     *
     * @since 5.78
     */
    void indexTriggered(int index);

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 78)
    /**
     * This signal is emitted when an item is selected.
     * @param text indicates the item selected
     * @deprecated Since 5.78, use textTriggered(const QString &)
     */
    KWIDGETSADDONS_DEPRECATED_VERSION(5, 78, "Use KSelectAction::textTriggered(const QString &)")
    void triggered(const QString &text); // clazy:exclude=overloaded-signal
#endif

    /**
     * This signal is emitted when an item is selected.
     * @param text indicates the item selected
     *
     * In your KSelectAction subclass to be backward-compatible to KF < 5.78, emit instead
     * just the deprecated signal triggered(const QString &). That will also
     * automatically emit textTriggered(const QString &) because this signal
     * is connected to the deprecated one in the KSelectAction constructor.
     * Only if you compile against a variant of KWidgetsAddons built without all API
     * deprecated up to version 5.78, then emit this signal directly.
     * Your code would be like this:
     * @code
     * #if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 78)
     *     // will also indirectly emit textTriggered since 5.78
     *     Q_EMIT triggered(action->text());
     * #else
     *     Q_EMIT textTriggered(action->text());
     * #endif
     * @endcode
     *
     * @since 5.78
     */
    void textTriggered(const QString &text);

protected Q_SLOTS:
    /**
     * This function is called whenever an action from the selections is triggered.
     * The default implementation calls trigger() if isCheckable() is @c true, then emits
     * the signals triggered(QAction *), triggered(int) and triggered(const QString &)
     * as well as since 5.78 the signals indexTriggered(int) and textTriggered(const QString &).
     */
    virtual void actionTriggered(QAction *action);
    // TODO KF6:: rename to handleActionTriggered, to release name to signal

    /**
     * For structured menu building. Deselects all items if the action was unchecked by the top menu
     */
    void slotToggled(bool);

protected:
    /**
     * Reimplemented from QWidgetAction.
     */
    QWidget *createWidget(QWidget *parent) override;

    /**
     * Reimplemented from QWidgetAction.
     */
    void deleteWidget(QWidget *widget) override;

    bool event(QEvent *event) override;

    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @internal
     * Creates a new KSelectAction object.
     *
     * @param dd the private d member
     * @param parent The action's parent object.
     */
    KSelectAction(KSelectActionPrivate &dd, QObject *parent);

    std::unique_ptr<class KSelectActionPrivate> const d_ptr;

private:
    Q_DECLARE_PRIVATE(KSelectAction)
};

#endif
