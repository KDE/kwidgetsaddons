/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "kpasswordlineedittest.h"
#include "kpasswordlineedit.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTest>
#include <QAction>

PasswordLineEditTest::PasswordLineEditTest(QObject *parent)
    : QObject(parent)
{

}

void PasswordLineEditTest::shouldHaveDefaultValue()
{
    KPasswordLineEdit lineEdit;
    QVERIFY(lineEdit.password().isEmpty());

    QHBoxLayout *mainLayout = lineEdit.findChild<QHBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->margin(), 0);

    QLineEdit *edit = lineEdit.findChild<QLineEdit *>(QStringLiteral("passwordlineedit"));
    QVERIFY(edit);
    QVERIFY(edit->text().isEmpty());
    QCOMPARE(edit->echoMode(), QLineEdit::Password);

    QVERIFY(lineEdit.toggleEchoModeAction());
    QVERIFY(!lineEdit.toggleEchoModeAction()->isVisible());
}

void PasswordLineEditTest::shouldShowTogglePassword()
{
    KPasswordLineEdit lineEdit;
    lineEdit.show();
    QTest::qWaitForWindowExposed(&lineEdit);

    QLineEdit *edit = lineEdit.findChild<QLineEdit *>(QStringLiteral("passwordlineedit"));
    edit->setText(QStringLiteral("FOO"));
    QVERIFY(lineEdit.toggleEchoModeAction()->isVisible());

    edit->clear();
    QVERIFY(!lineEdit.toggleEchoModeAction()->isVisible());
}

void PasswordLineEditTest::shouldNotShowToggleWhenSetPassword()
{
    KPasswordLineEdit lineEdit;
    lineEdit.show();
    QTest::qWaitForWindowExposed(&lineEdit);
    lineEdit.setPassword(QStringLiteral("foo"));
    QVERIFY(!lineEdit.toggleEchoModeAction()->isVisible());
}

void PasswordLineEditTest::shouldShowRevealPassword()
{
    KPasswordLineEdit lineEdit;
    lineEdit.show();
    QTest::qWaitForWindowExposed(&lineEdit);

    QLineEdit *edit = lineEdit.findChild<QLineEdit *>(QStringLiteral("passwordlineedit"));
    edit->setText(QStringLiteral("FOO"));
    QVERIFY(lineEdit.toggleEchoModeAction()->isVisible());

    lineEdit.setRevealPasswordAvailable(false);
    QVERIFY(!lineEdit.toggleEchoModeAction()->isVisible());

    lineEdit.setRevealPasswordAvailable(true);
    QVERIFY(lineEdit.toggleEchoModeAction()->isVisible());

    edit->clear();
    QVERIFY(!lineEdit.toggleEchoModeAction()->isVisible());
}

QTEST_MAIN(PasswordLineEditTest)
