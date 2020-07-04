/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 1999 Reginald Stadlbauer <reggie@kde.org>
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 2000 Nicolas Hadacek <haadcek@kde.org>
    SPDX-FileCopyrightText: 2000 Kurt Granroth <granroth@kde.org>
    SPDX-FileCopyrightText: 2000 Michael Koch <koch@kde.org>
    SPDX-FileCopyrightText: 2001 Holger Freyther <freyther@kde.org>
    SPDX-FileCopyrightText: 2002 Ellis Whitehead <ellis@kde.org>
    SPDX-FileCopyrightText: 2002 Joseph Wenninger <jowenn@kde.org>
    SPDX-FileCopyrightText: 2003 Andras Mantia <amantia@kde.org>
    SPDX-FileCopyrightText: 2005-2006 Hamish Rodda <rodda@kde.org>

    SPDX-License-Identifier: LGPL-2.0-only
*/

#include "kactionmenu.h"

#include <QMenu>
#include <QToolButton>
#include <QToolBar>

class KActionMenuPrivate
{
public:
    KActionMenuPrivate()
    {
    }
    ~KActionMenuPrivate()
    {
    }
    bool m_delayed = true;
    bool m_stickyMenu = true;
};

KActionMenu::KActionMenu(QObject *parent)
    : QWidgetAction(parent)
    , d(new KActionMenuPrivate)
{
    setMenu(new QMenu);
    setProperty("isShortcutConfigurable", false);
}

KActionMenu::KActionMenu(const QString &text, QObject *parent)
    : QWidgetAction(parent)
    , d(new KActionMenuPrivate)
{
    setMenu(new QMenu);
    setProperty("isShortcutConfigurable", false);
    setText(text);
}

KActionMenu::KActionMenu(const QIcon &icon, const QString &text, QObject *parent)
    : QWidgetAction(parent)
    , d(new KActionMenuPrivate)
{
    setMenu(new QMenu);
    setProperty("isShortcutConfigurable", false);
    setIcon(icon);
    setText(text);
}

KActionMenu::~KActionMenu()
{
    delete d;
    delete menu();
}

QWidget *KActionMenu::createWidget(QWidget *_parent)
{
    QToolBar *parent = qobject_cast<QToolBar *>(_parent);
    if (!parent) {
        return QWidgetAction::createWidget(_parent);
    }
    QToolButton *button = new QToolButton(parent);
    button->setAutoRaise(true);
    button->setFocusPolicy(Qt::NoFocus);
    button->setIconSize(parent->iconSize());
    button->setToolButtonStyle(parent->toolButtonStyle());
    QObject::connect(parent, &QToolBar::iconSizeChanged,
                     button, &QAbstractButton::setIconSize);
    QObject::connect(parent, &QToolBar::toolButtonStyleChanged,
                     button, &QToolButton::setToolButtonStyle);
    button->setDefaultAction(this);
    QObject::connect(button, &QToolButton::triggered, parent, &QToolBar::actionTriggered);

    if (delayed()) {
        button->setPopupMode(QToolButton::DelayedPopup);
    } else if (stickyMenu()) {
        button->setPopupMode(QToolButton::InstantPopup);
    } else {
        button->setPopupMode(QToolButton::MenuButtonPopup);
    }

    return button;
}

#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 0)
void KActionMenu::remove(QAction *cmd)
{
    if (cmd) {
        menu()->removeAction(cmd);
    }
}
#endif

void KActionMenu::addAction(QAction *action)
{
    menu()->addAction(action);
}

QAction *KActionMenu::addSeparator()
{
    QAction *separator = new QAction(this);
    separator->setSeparator(true);
    addAction(separator);
    return separator;
}

QAction *KActionMenu::insertSeparator(QAction *before)
{
    QAction *separator = new QAction(this);
    separator->setSeparator(true);
    insertAction(before, separator);
    return separator;
}

void KActionMenu::insertAction(QAction *before, QAction *action)
{
    menu()->insertAction(before, action);
}

void KActionMenu::removeAction(QAction *action)
{
    menu()->removeAction(action);
}

bool KActionMenu::delayed() const
{
    return d->m_delayed;
}

void KActionMenu::setDelayed(bool _delayed)
{
    d->m_delayed = _delayed;
}

bool KActionMenu::stickyMenu() const
{
    return d->m_stickyMenu;
}

void KActionMenu::setStickyMenu(bool sticky)
{
    d->m_stickyMenu = sticky;
}

