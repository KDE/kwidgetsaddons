/*
    SPDX-FileCopyrightText: 2013 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kcolorbuttontest.h"

#include <QTest>
#include <kcolorbutton.h>
#include <QColorDialog>

QTEST_MAIN(KColorButtonTest)

void KColorButtonTest::testWaitForWindowExposed()
{
    // because this test fails in the Windows CI, in qWaitForWindowExposed
    // let's test that here
    QWindow a;
    a.setVisible(true);
    QVERIFY(QTest::qWaitForWindowExposed(&a));
}

void KColorButtonTest::testOpenDialog()
{
    KColorButton colorButton(Qt::red);
    colorButton.show();
    QVERIFY(QTest::qWaitForWindowExposed(&colorButton));
    QTest::mouseClick(&colorButton, Qt::LeftButton);
    QColorDialog *dialog = colorButton.findChild<QColorDialog *>();
    QVERIFY(dialog != nullptr);
    QVERIFY(QTest::qWaitForWindowExposed(dialog));
    QCOMPARE(dialog->currentColor(), QColor(Qt::red));
}
