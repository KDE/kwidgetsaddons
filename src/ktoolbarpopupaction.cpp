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

#include "ktoolbarpopupaction.h"

#include <QMenu>
#include <QToolBar>
#include <QToolButton>

class Q_DECL_HIDDEN KToolBarPopupAction::Private
{
public:
    Private()
        : delayed(true), stickyMenu(true)
    {
    }

    bool delayed: 1;
    bool stickyMenu: 1;
};

KToolBarPopupAction::KToolBarPopupAction(const QIcon &icon, const QString &text, QObject *parent)
    : QWidgetAction(parent),
      d(new Private)
{
    setIcon(icon);
    setText(text);
    setMenu(new QMenu);
}

KToolBarPopupAction::~KToolBarPopupAction()
{
    delete d;
    delete menu();
}

#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 0)
QMenu *KToolBarPopupAction::popupMenu() const
{
    return menu();
}
#endif

QWidget *KToolBarPopupAction::createWidget(QWidget *_parent)
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
    button->setDefaultAction(this);

    connect(parent, &QToolBar::iconSizeChanged,
            button, &QAbstractButton::setIconSize);
    connect(parent, &QToolBar::toolButtonStyleChanged,
            button, &QToolButton::setToolButtonStyle);
    connect(button, &QToolButton::triggered,
            parent, &QToolBar::actionTriggered);

    if (d->delayed)
        if (d->stickyMenu) {
            button->setPopupMode(QToolButton::MenuButtonPopup);
        } else {
            button->setPopupMode(QToolButton::DelayedPopup);
        }
    else {
        button->setPopupMode(QToolButton::InstantPopup);
    }

    return button;
}

bool KToolBarPopupAction::delayed() const
{
    return d->delayed;
}

void KToolBarPopupAction::setDelayed(bool delayed)
{
    d->delayed = delayed;
}

bool KToolBarPopupAction::stickyMenu() const
{
    return d->stickyMenu;
}

void KToolBarPopupAction::setStickyMenu(bool sticky)
{
    d->stickyMenu = sticky;
}

