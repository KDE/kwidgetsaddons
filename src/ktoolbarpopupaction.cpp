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

class KToolBarPopupActionPrivate
{
public:
    KToolBarPopupActionPrivate()
        : m_popupMode(QToolButton::MenuButtonPopup)
        , m_popupMenu(new QMenu())
    {
    }

    QToolButton::ToolButtonPopupMode m_popupMode;
    QMenu *m_popupMenu;
};

KToolBarPopupAction::KToolBarPopupAction(const QIcon &icon, const QString &text, QObject *parent)
    : QWidgetAction(parent)
    , d(new KToolBarPopupActionPrivate)
{
    setIcon(icon);
    setText(text);
}

KToolBarPopupAction::~KToolBarPopupAction()
{
    delete d->m_popupMenu;
}

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
    button->setPopupMode(d->m_popupMode);
    button->setMenu(d->m_popupMenu);

    connect(parent, &QToolBar::iconSizeChanged, button, &QAbstractButton::setIconSize);
    connect(parent, &QToolBar::toolButtonStyleChanged, button, &QToolButton::setToolButtonStyle);
    connect(button, &QToolButton::triggered, parent, &QToolBar::actionTriggered);

    return button;
}

QToolButton::ToolButtonPopupMode KToolBarPopupAction::popupMode() const
{
    return d->m_popupMode;
}

void KToolBarPopupAction::setPopupMode(QToolButton::ToolButtonPopupMode popupMode)
{
    d->m_popupMode = popupMode;

    const auto widgets = createdWidgets();
    for (QWidget *widget : widgets) {
        if (auto *button = qobject_cast<QToolButton *>(widget)) {
            button->setPopupMode(d->m_popupMode);
        }
    }
}

QMenu *KToolBarPopupAction::popupMenu() const
{
    return d->m_popupMenu;
}

#include "moc_ktoolbarpopupaction.cpp"
