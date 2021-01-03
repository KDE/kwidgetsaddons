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

#include "ktoggleaction.h"

#include <kguiitem.h>

class Q_DECL_HIDDEN KToggleAction::Private
{
public:
    Private(KToggleAction *_parent)
        : parent(_parent)
    {
    }

    ~Private()
    {
        delete checkedGuiItem;
    }

    void init()
    {
        parent->setCheckable(true);
        connect(parent, &QAction::toggled,
                parent, &KToggleAction::slotToggled);
    }

    KToggleAction *const parent;
    KGuiItem *checkedGuiItem = nullptr;
};

KToggleAction::KToggleAction(QObject *parent)
    : QAction(parent),
      d(new Private(this))
{
    d->init();
}

KToggleAction::KToggleAction(const QString &text, QObject *parent)
    : QAction(parent),
      d(new Private(this))
{
    setText(text);
    d->init();
}

KToggleAction::KToggleAction(const QIcon &icon, const QString &text, QObject *parent)
    : QAction(parent),
      d(new Private(this))
{
    setIcon(icon);
    setText(text);
    d->init();
}

KToggleAction::~KToggleAction() = default;

void KToggleAction::setCheckedState(const KGuiItem &checkedItem)
{
    delete d->checkedGuiItem;
    d->checkedGuiItem = new KGuiItem(checkedItem);
}

void KToggleAction::slotToggled(bool)
{
    if (d->checkedGuiItem) {
        QString string = d->checkedGuiItem->text();
        d->checkedGuiItem->setText(text());
        setText(string);

        string = d->checkedGuiItem->toolTip();
        d->checkedGuiItem->setToolTip(toolTip());
        setToolTip(string);

        if (d->checkedGuiItem->hasIcon()) {
            QIcon icon = d->checkedGuiItem->icon();
            d->checkedGuiItem->setIcon(this->icon());
            QAction::setIcon(icon);
        }
    }
}

