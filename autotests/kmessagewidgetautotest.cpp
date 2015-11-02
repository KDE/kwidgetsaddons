/*
      Copyright 2014 Dominik Haumann <dhaumann@kde.org>

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

#include <QtTest/QtTest>
#include <kmessagewidget.h>

QTEST_MAIN(KMessageWidgetTest)

void KMessageWidgetTest::initTestCase()
{
    m_hideSignals = 0;
    m_showSignals = 0;
}

void KMessageWidgetTest::testAnimationSignals()
{
    KMessageWidget w(QStringLiteral("Hello world"));

    connect(&w, &KMessageWidget::showAnimationFinished, this, &KMessageWidgetTest::showAnimationFinished);
    connect(&w, &KMessageWidget::hideAnimationFinished, this, &KMessageWidgetTest::hideAnimationFinished);

    QCOMPARE(m_showSignals, 0);

    //
    // test: showing the message widget should emit showAnimationFinished()
    // exactly once after the show animation is finished
    //
    w.animatedShow();

    while (w.isShowAnimationRunning()) {
        QCOMPARE(m_showSignals, 0);
        QTest::qWait(50);
    }

    QVERIFY(! w.isShowAnimationRunning());
    QCOMPARE(m_showSignals, 1);

    //
    // test: hiding the message widget should emit hideAnimationFinished()
    // exactly once after the show animation is finished
    //
    w.animatedHide();

    while (w.isHideAnimationRunning()) {
        QCOMPARE(m_hideSignals, 0);
        QTest::qWait(50);
    }

    QVERIFY(! w.isHideAnimationRunning());
    QCOMPARE(m_hideSignals, 1);
}

void KMessageWidgetTest::hideAnimationFinished()
{
    ++m_hideSignals;
}

void KMessageWidgetTest::showAnimationFinished()
{
    ++m_showSignals;
}
