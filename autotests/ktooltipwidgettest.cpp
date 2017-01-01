/*
 * Copyright (c) 2017 Elvis Angelaccio <elvis.angelaccio@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ktooltipwidgettest.h"

#include <KToolTipWidget>

#include <QLabel>
#include <QSignalSpy>
#include <QTest>

void KToolTipWidgetTest::showTooltipShouldShowContent()
{
    KToolTipWidget tooltip;
    QLabel label;

    tooltip.showAt(QPoint(10, 10), &label, nullptr);
    QVERIFY(tooltip.isVisible());
    QVERIFY(label.isVisible());
}

void KToolTipWidgetTest::hideLaterShouldHideAfterDelay()
{
    KToolTipWidget tooltip;
    QLabel label;

    tooltip.showAt(QPoint(10, 10), &label, nullptr);
    QVERIFY(tooltip.isVisible());
    QVERIFY(label.isVisible());

    tooltip.hideLater();
    QVERIFY(tooltip.isVisible());
    QVERIFY(label.isVisible());

    QSignalSpy spy(&tooltip, &KToolTipWidget::hidden);
    connect(&tooltip, &KToolTipWidget::hidden, this, [&]() {
        QVERIFY(tooltip.isHidden());
        QVERIFY(!label.isVisible());
    });

    QEventLoop eventLoop;
    connect(&tooltip, &KToolTipWidget::hidden, &eventLoop, &QEventLoop::quit);
    QCOMPARE(eventLoop.exec(), 0);
    QCOMPARE(spy.count(), 1);
}

void KToolTipWidgetTest::setZeroDelayShouldHideImmediately()
{
    KToolTipWidget tooltip;
    QLabel label;

    tooltip.setHideDelay(0);
    tooltip.showAt(QPoint(10, 10), &label, nullptr);
    QVERIFY(tooltip.isVisible());
    QVERIFY(label.isVisible());

    tooltip.hideLater();
    QVERIFY(tooltip.isHidden());
    QVERIFY(!label.isVisible());
}

QTEST_MAIN(KToolTipWidgetTest)


