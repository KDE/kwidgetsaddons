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
#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 78)
        , stickyMenu(true)
#endif
    {
    }

    QToolButton::ToolButtonPopupMode m_popupMode;
#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 78)
    bool stickyMenu : 1;
#endif
};

KToolBarPopupAction::KToolBarPopupAction(const QIcon &icon, const QString &text, QObject *parent)
    : QWidgetAction(parent)
    , d(new KToolBarPopupActionPrivate)
{
    setIcon(icon);
    setText(text);
    setMenu(new QMenu);
}

KToolBarPopupAction::~KToolBarPopupAction()
{
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
    button->setPopupMode(d->m_popupMode);

    connect(parent, &QToolBar::iconSizeChanged, button, &QAbstractButton::setIconSize);
    connect(parent, &QToolBar::toolButtonStyleChanged, button, &QToolButton::setToolButtonStyle);
    connect(button, &QToolButton::triggered, parent, &QToolBar::actionTriggered);

    return button;
}

#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 78)
bool KToolBarPopupAction::delayed() const
{
    return popupMode() != QToolButton::InstantPopup;
}
#endif

#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 78)
void KToolBarPopupAction::setDelayed(bool delayed)
{
    if (delayed) {
        if (d->stickyMenu) {
            setPopupMode(QToolButton::MenuButtonPopup);
        } else {
            setPopupMode(QToolButton::DelayedPopup);
        }
    } else {
        setPopupMode(QToolButton::InstantPopup);
    }
}
#endif

#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 78)
bool KToolBarPopupAction::stickyMenu() const
{
    if (popupMode() == QToolButton::InstantPopup) {
        return d->stickyMenu;
    } else if (popupMode() == QToolButton::MenuButtonPopup) {
        return true;
    } else { // DelayedPopup
        return false;
    }
}
#endif

#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 78)
void KToolBarPopupAction::setStickyMenu(bool sticky)
{
    if (popupMode() != QToolButton::InstantPopup) {
        if (sticky) {
            setPopupMode(QToolButton::MenuButtonPopup);
        } else {
            setPopupMode(QToolButton::DelayedPopup);
        }
    }

    d->stickyMenu = sticky;
}
#endif

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

#include "moc_ktoolbarpopupaction.cpp"
