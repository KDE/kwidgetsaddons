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

#include "kfontsizeaction.h"

#include "loggingcategory.h"

#include <QFontDatabase>

class Q_DECL_HIDDEN KFontSizeAction::Private
{
public:
    Private(KFontSizeAction *parent)
        : q(parent)
    {
    }

    void init();

    KFontSizeAction *const q;
};

// BEGIN KFontSizeAction
KFontSizeAction::KFontSizeAction(QObject *parent)
    : KSelectAction(parent),
      d(new Private(this))
{
    d->init();
}

KFontSizeAction::KFontSizeAction(const QString &text, QObject *parent)
    : KSelectAction(text, parent),
      d(new Private(this))
{
    d->init();
}

KFontSizeAction::KFontSizeAction(const QIcon &icon, const QString &text, QObject *parent)
    : KSelectAction(icon, text, parent),
      d(new Private(this))
{
    d->init();
}

KFontSizeAction::~KFontSizeAction() = default;

void KFontSizeAction::Private::init()
{
    q->setEditable(true);
    QFontDatabase fontDB;
    const QList<int> sizes = fontDB.standardSizes();
    QStringList lst;
    lst.reserve(sizes.count());
    for (QList<int>::ConstIterator it = sizes.begin(), total = sizes.end(); it != total; ++it) {
        lst.append(QString::number(*it));
    }

    q->setItems(lst);
}

void KFontSizeAction::setFontSize(int size)
{
    if (size == fontSize()) {
        const QString test = QString::number(size);
        const auto actions = this->actions();
        for (QAction *action : actions) {
            if (action->text() == test) {
                setCurrentAction(action);
                return;
            }
        }
    }

    if (size < 1) {
        qCWarning(KWidgetsAddonsLog) << "KFontSizeAction: Size " << size << " is out of range";
        return;
    }

    QAction *a = action(QString::number(size));
    if (!a) {
        // Insert at the correct position in the list (to keep sorting)
        QList<int> lst;
        // Convert to list of ints
        QStringListIterator itemsIt(items());
        while (itemsIt.hasNext()) {
            lst.append(itemsIt.next().toInt());
        }
        // New size
        lst.append(size);
        // Sort the list
        std::sort(lst.begin(), lst.end());
        clear();
        for (int it : qAsConst(lst)) {
            QAction *const action = addAction(QString::number(it));
            if (it == size) {
                setCurrentAction(action);
            }
        }

    } else {
        setCurrentAction(a);
    }
}

int KFontSizeAction::fontSize() const
{
    return currentText().toInt();
}

void KFontSizeAction::actionTriggered(QAction *action)
{
    emit fontSizeChanged(action->text().toInt());
    KSelectAction::actionTriggered(action);
}

