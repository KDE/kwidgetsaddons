/*
    SPDX-FileCopyrightText: 2017 Montel Laurent <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "kpasswordlineedittest.h"
#include "kpasswordlineedit.h"
#include <QAction>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSignalSpy>
#include <QTest>

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
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

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
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

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
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));
    lineEdit.setPassword(QStringLiteral("foo"));
    QVERIFY(!lineEdit.toggleEchoModeAction()->isVisible());
}

void PasswordLineEditTest::shouldShowRevealPassword()
{
    KPasswordLineEdit lineEdit;
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    QLineEdit *edit = lineEdit.findChild<QLineEdit *>(QStringLiteral("passwordlineedit"));
    edit->setText(QStringLiteral("FOO"));
    QVERIFY(lineEdit.toggleEchoModeAction()->isVisible());

    lineEdit.setRevealPasswordMode(KPasswordLineEdit::RevealPasswordMode::Never);
    QVERIFY(!lineEdit.toggleEchoModeAction()->isVisible());

    lineEdit.setRevealPasswordMode(KPasswordLineEdit::RevealPasswordMode::Always);
    QVERIFY(lineEdit.toggleEchoModeAction()->isVisible());

    lineEdit.setRevealPasswordMode(KPasswordLineEdit::RevealPasswordMode::Normal);
    QVERIFY(lineEdit.toggleEchoModeAction()->isVisible());

    lineEdit.setPassword(QStringLiteral("FOO2"));
    QVERIFY(!lineEdit.toggleEchoModeAction()->isVisible());

    edit->clear();
    QVERIFY(!lineEdit.toggleEchoModeAction()->isVisible());
}

void PasswordLineEditTest::shouldEmitSignalPasswordChanged()
{
    KPasswordLineEdit lineEdit;
    lineEdit.show();
    QSignalSpy spy(&lineEdit, &KPasswordLineEdit::passwordChanged);
    lineEdit.setPassword(QStringLiteral("foo"));
    QCOMPARE(spy.count(), 1);
}

void PasswordLineEditTest::testReadOnly()
{
    KPasswordLineEdit lineEdit;
    lineEdit.show();
    lineEdit.setReadOnly(true);
    QSignalSpy spy(&lineEdit, &KPasswordLineEdit::passwordChanged);
    lineEdit.setPassword(QStringLiteral("foo"));
    QCOMPARE(spy.count(), 1);
    QTest::keyClick(&lineEdit, Qt::Key_A);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(lineEdit.password(), QStringLiteral("foo"));
}

QTEST_MAIN(PasswordLineEditTest)

#include "moc_kpasswordlineedittest.cpp"
