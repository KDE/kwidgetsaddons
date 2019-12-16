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

#include <QTest>
#include <QSignalSpy>
#include <kmessagewidget.h>

QTEST_MAIN(KMessageWidgetTest)

// KMessageWidget is currently hardcoded to a 500 ms timeline and default QTimeLine 40 ms update interval
// let's have 7 updates for now, should be save
const int overlappingWaitingTime = 280;

#define CHECK_FULLY_VISIBLE \
    QVERIFY(w.isVisible()); \
    QCOMPARE(w.height(), w.sizeHint().height()); \
    QCOMPARE(w.findChild<QWidget*>(QStringLiteral("contentWidget")) ->pos(), QPoint(0, 0));

#define CHECK_FULLY_NOT_VISIBLE \
    QCOMPARE(w.height(), 0); \
    QCOMPARE(w.findChild<QWidget*>(QStringLiteral("contentWidget")) ->pos(), QPoint(0, -w.sizeHint().height())); \
    QVERIFY(!w.isVisible());


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

    QTRY_VERIFY(!w.isShowAnimationRunning());
    QCOMPARE(showSignalsSpy.count(), 1);
    CHECK_FULLY_VISIBLE

    //
    // test: hiding the message widget should emit hideAnimationFinished()
    // exactly once after the show animation is finished
    //
    w.animatedHide();

    QTRY_VERIFY(!w.isHideAnimationRunning());
    QCOMPARE(hideSignalsSpy.count(), 1);
    CHECK_FULLY_NOT_VISIBLE
}

void KMessageWidgetTest::testShowOnVisible()
{
    KMessageWidget w(QStringLiteral("Hello world"));
    QSignalSpy showSignalsSpy(&w, &KMessageWidget::showAnimationFinished);
    w.show();
    CHECK_FULLY_VISIBLE

    // test: call show on visible
    w.animatedShow();

    QTRY_VERIFY(!w.isShowAnimationRunning());
    QCOMPARE(showSignalsSpy.count(), 1);
    CHECK_FULLY_VISIBLE
}

void KMessageWidgetTest::testHideOnInvisible()
{
    KMessageWidget w(QStringLiteral("Hello world"));
    QSignalSpy hideSignalsSpy(&w, &KMessageWidget::hideAnimationFinished);

    // test: call hide on invisible
    w.animatedHide();

    QTRY_VERIFY(!w.isHideAnimationRunning());
    QCOMPARE(hideSignalsSpy.count(), 1);
    QVERIFY(! w.isVisible()); // Not CHECK_FULLY_NOT_VISIBLE because since it was never shown height is not what it would be otherwise
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
    QVERIFY(w.isVisible()); // Not CHECK_FULLY_VISIBLE because we're not waiting for it to finish
    if (delay) {
        QTest::qWait(overlappingWaitingTime);
    }
    w.animatedHide();

    QVERIFY(! w.isShowAnimationRunning());
    QCOMPARE(showSignalsSpy.count(), 1);

    QTRY_VERIFY(!w.isHideAnimationRunning());
    QCOMPARE(hideSignalsSpy.count(), 1);
    CHECK_FULLY_NOT_VISIBLE
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
    CHECK_FULLY_VISIBLE

    // test: call show while hide animation still running
    w.animatedHide();
    if (delay) {
        QTest::qWait(overlappingWaitingTime);
    }
    w.animatedShow();

    QVERIFY(! w.isHideAnimationRunning());
    QCOMPARE(hideSignalsSpy.count(), 1);

    QTRY_VERIFY(!w.isShowAnimationRunning());
    QCOMPARE(showSignalsSpy.count(), 1);
    CHECK_FULLY_VISIBLE
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

    QTRY_VERIFY(!w.isShowAnimationRunning());
    QCOMPARE(showSignalsSpy.count(), 1);
    CHECK_FULLY_VISIBLE
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

    QTRY_VERIFY(!w.isHideAnimationRunning());
    QCOMPARE(hideSignalsSpy.count(), 1);
    CHECK_FULLY_NOT_VISIBLE
}

void KMessageWidgetTest::testHideWithNotYetShownParent()
{
    QWidget parent;
    KMessageWidget w(QStringLiteral("Hello world"), &parent);
    QSignalSpy hideSignalsSpy(&w, &KMessageWidget::hideAnimationFinished);

    // test: call hide while not yet visible
    w.animatedHide();

    QTRY_VERIFY(!w.isHideAnimationRunning());
    parent.show();

    QCOMPARE(hideSignalsSpy.count(), 1);
    QVERIFY(! w.isVisible()); // Not CHECK_FULLY_NOT_VISIBLE because since it was never shown height is not what it would be otherwise
}

void KMessageWidgetTest::testNonAnimatedShowAfterAnimatedHide()
{
    KMessageWidget w(QStringLiteral("Hello world"));
    QSignalSpy hideSignalsSpy(&w, &KMessageWidget::hideAnimationFinished);

    w.show();
    CHECK_FULLY_VISIBLE

    w.animatedHide();

    QTRY_VERIFY(!w.isHideAnimationRunning());

    QCOMPARE(hideSignalsSpy.count(), 1);
    CHECK_FULLY_NOT_VISIBLE

    w.show();
    CHECK_FULLY_VISIBLE
}
