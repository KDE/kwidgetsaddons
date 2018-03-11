/*
      Copyright 2014 Dominik Haumann <dhaumann@kde.org>
      Copyright 2017 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kmessagewidgetautotest.h"

#include <QtTest>
#include <kmessagewidget.h>

QTEST_MAIN(KMessageWidgetTest)

// KMessageWidget is currently hardcoded to a 500 ms timeline and default QTimeLine 40 ms update intervall
// let's have 7 updates for now, should be save
const int overlappingWaitingTime = 280;

void KMessageWidgetTest::testAnimationSignals()
{
    KMessageWidget w(QStringLiteral("Hello world"));

    QSignalSpy showSignalsSpy(&w, &KMessageWidget::showAnimationFinished);
    QSignalSpy hideSignalsSpy(&w, &KMessageWidget::hideAnimationFinished);

    QCOMPARE(showSignalsSpy.count(), 0);

    //
    // test: showing the message widget should emit showAnimationFinished()
    // exactly once after the show animation is finished
    //
    w.animatedShow();

    while (w.isShowAnimationRunning()) {
        QCOMPARE(showSignalsSpy.count(), 0);
        QTest::qWait(50);
    }

    QVERIFY(! w.isShowAnimationRunning());
    QCOMPARE(showSignalsSpy.count(), 1);
    QVERIFY(w.isVisible());

    //
    // test: hiding the message widget should emit hideAnimationFinished()
    // exactly once after the show animation is finished
    //
    w.animatedHide();

    while (w.isHideAnimationRunning()) {
        QCOMPARE(hideSignalsSpy.count(), 0);
        QTest::qWait(50);
    }

    QCOMPARE(hideSignalsSpy.count(), 1);
    QVERIFY(! w.isVisible());
}

void KMessageWidgetTest::testShowOnVisible()
{
    KMessageWidget w(QStringLiteral("Hello world"));
    QSignalSpy showSignalsSpy(&w, &KMessageWidget::showAnimationFinished);
    w.show();

    // test: call show on visible
    w.animatedShow();

    while (w.isShowAnimationRunning()) {
        QCOMPARE(showSignalsSpy.count(), 0);
        QTest::qWait(50);
    }

    QCOMPARE(showSignalsSpy.count(), 1);
    QVERIFY(w.isVisible());
}

void KMessageWidgetTest::testHideOnInvisible()
{
    KMessageWidget w(QStringLiteral("Hello world"));
    QSignalSpy hideSignalsSpy(&w, &KMessageWidget::hideAnimationFinished);

    // test: call hide on visible
    w.animatedHide();

    while (w.isHideAnimationRunning()) {
        QCOMPARE(hideSignalsSpy.count(), 0);
        QTest::qWait(50);
    }

    QCOMPARE(hideSignalsSpy.count(), 1);
    QVERIFY(! w.isVisible());
}

void KMessageWidgetTest::testOverlappingShowAndHide_data()
{
    QTest::addColumn<bool>("delay");
    QTest::newRow("instant") << false;
    QTest::newRow("delay")   << true;
}

void KMessageWidgetTest::testOverlappingShowAndHide()
{
    QFETCH(bool, delay);

    KMessageWidget w(QStringLiteral("Hello world"));

    QSignalSpy showSignalsSpy(&w, &KMessageWidget::showAnimationFinished);
    QSignalSpy hideSignalsSpy(&w, &KMessageWidget::hideAnimationFinished);

    // test: call hide while show animation still running
    w.animatedShow();
    if (delay) {
        QTest::qWait(overlappingWaitingTime);
    }
    w.animatedHide();

    QVERIFY(! w.isShowAnimationRunning());
    QCOMPARE(showSignalsSpy.count(), 1);

    while (w.isHideAnimationRunning()) {
        QCOMPARE(hideSignalsSpy.count(), 0);
        QTest::qWait(50);
    }

    QCOMPARE(hideSignalsSpy.count(), 1);
    QVERIFY(! w.isVisible());
}

void KMessageWidgetTest::testOverlappingHideAndShow_data()
{
    QTest::addColumn<bool>("delay");
    QTest::newRow("instant") << false;
    QTest::newRow("delay")   << true;
}

void KMessageWidgetTest::testOverlappingHideAndShow()
{
    QFETCH(bool, delay);

    KMessageWidget w(QStringLiteral("Hello world"));
    QSignalSpy showSignalsSpy(&w, &KMessageWidget::showAnimationFinished);
    QSignalSpy hideSignalsSpy(&w, &KMessageWidget::hideAnimationFinished);
    w.show();

    // test: call show while hide animation still running
    w.animatedHide();
    if (delay) {
        QTest::qWait(overlappingWaitingTime);
    }
    w.animatedShow();

    QVERIFY(! w.isHideAnimationRunning());
    QCOMPARE(hideSignalsSpy.count(), 1);

    while (w.isShowAnimationRunning()) {
        QCOMPARE(showSignalsSpy.count(), 0);
        QTest::qWait(50);
    }

    QCOMPARE(showSignalsSpy.count(), 1);
    QVERIFY(w.isVisible());
}

void KMessageWidgetTest::testOverlappingDoubleShow_data()
{
    QTest::addColumn<bool>("delay");
    QTest::newRow("instant") << false;
    QTest::newRow("delay")   << true;
}

void KMessageWidgetTest::testOverlappingDoubleShow()
{
    QFETCH(bool, delay);

    KMessageWidget w(QStringLiteral("Hello world"));
    QSignalSpy showSignalsSpy(&w, &KMessageWidget::showAnimationFinished);

    // test: call show while show animation still running
    w.animatedShow();
    if (delay) {
        QTest::qWait(overlappingWaitingTime);
    }
    w.animatedShow();

    while (w.isShowAnimationRunning()) {
        QCOMPARE(showSignalsSpy.count(), 0);
        QTest::qWait(50);
    }

    QCOMPARE(showSignalsSpy.count(), 1);
    QVERIFY(w.isVisible());
}

void KMessageWidgetTest::testOverlappingDoubleHide_data()
{
    QTest::addColumn<bool>("delay");
    QTest::newRow("instant") << false;
    QTest::newRow("delay")   << true;
}

void KMessageWidgetTest::testOverlappingDoubleHide()
{
    QFETCH(bool, delay);

    KMessageWidget w(QStringLiteral("Hello world"));
    QSignalSpy hideSignalsSpy(&w, &KMessageWidget::hideAnimationFinished);
    w.show();

    // test: call hide while hide animation still running
    w.animatedHide();
    if (delay) {
        QTest::qWait(overlappingWaitingTime);
    }
    w.animatedHide();

    while (w.isHideAnimationRunning()) {
        QCOMPARE(hideSignalsSpy.count(), 0);
        QTest::qWait(50);
    }

    QCOMPARE(hideSignalsSpy.count(), 1);
    QVERIFY(! w.isVisible());
}

void KMessageWidgetTest::testHideWithNotYetShownParent()
{
    QWidget parent;
    KMessageWidget w(QStringLiteral("Hello world"), &parent);
    QSignalSpy hideSignalsSpy(&w, &KMessageWidget::hideAnimationFinished);

    // test: call hide while not yet visible
    w.animatedHide();

    while (w.isHideAnimationRunning()) {
        QCOMPARE(hideSignalsSpy.count(), 0);
        QTest::qWait(50);
    }
    parent.show();

    QCOMPARE(hideSignalsSpy.count(), 1);
    QVERIFY(! w.isVisible());
}
