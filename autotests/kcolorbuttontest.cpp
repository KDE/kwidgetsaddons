/*
    SPDX-FileCopyrightText: 2013 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kcolorbuttontest.h"

#include <QColorDialog>
#include <QTest>
#include <kcolorbutton.h>

QTEST_MAIN(KColorButtonTest)

void KColorButtonTest::testWaitForWindowExposed()
{
    // because this test fails in the Windows CI, in qWaitForWindowExposed
    // let's test that here
    QWindow a;
    a.setVisible(true);
    // Docu says: Note that a window that is mapped to screen may still not be considered exposed if the window client
    // area is completely covered by other windows, or if the window is otherwise not visible.
    qDebug() << "isExposed=" << a.isExposed() << "geometry=" << a.geometry() << "visible=" << a.isVisible(); // for Windows (immediate)
#ifdef Q_OS_WIN
    if (!a.isExposed()) {
        a.setGeometry(QRect(0, 0, 1000, 1000));
        qDebug() << "with updated geometry:"
                 << "isExposed=" << a.isExposed() << "geometry=" << a.geometry();
    }
#endif
    QVERIFY(QTest::qWaitForWindowExposed(&a));
    qDebug() << "isExposed=" << a.isExposed() << "geometry=" << a.geometry() << "visible=" << a.isVisible(); // for X11 (delayed)
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
