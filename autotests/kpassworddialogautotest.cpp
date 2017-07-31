/*
    Copyright 2017 Elvis Angelaccio <elvis.angelaccio@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "kpassworddialogautotest.h"

#include <QAction>
#include <QLineEdit>
#include <QTest>

#include <KPasswordDialog>
#include <KPasswordLineEdit>

QTEST_MAIN(KPasswordDialogAutotest)

void KPasswordDialogAutotest::shouldNotHideVisibilityActionInPlaintextMode()
{
    KPasswordDialog dialog;

    auto linePassword = dialog.findChild<KPasswordLineEdit*>(QStringLiteral("passEdit"));
    QVERIFY(linePassword);

    auto lineEdit = linePassword->lineEdit();
    QVERIFY(lineEdit);

    auto visibilityAction = lineEdit->findChild<QAction*>(QStringLiteral("visibilityAction"));
    QVERIFY(visibilityAction && !visibilityAction->isVisible());

    linePassword->lineEdit()->setText(QStringLiteral("1234"));
    QVERIFY(visibilityAction->isVisible());

    visibilityAction->trigger();
    linePassword->clear();
    QVERIFY(visibilityAction->isVisible());
}

