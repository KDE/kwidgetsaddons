/*
 * SPDX-FileCopyrightText: 2025 Kai Uwe Broulik <kde@broulik.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "kiconwidgettest.h"

#include <KIconWidget>
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include <QStyle>

#include <QTest>

#include <memory>

namespace
{

QPixmap createPixmap(const QSize &logicalSize, qreal dpr)
{
    QPixmap pixmap{logicalSize * dpr};
    pixmap.setDevicePixelRatio(dpr);
    pixmap.fill(Qt::magenta);
    return pixmap;
}

} // namespace

void KIconWidgetTest::testDefaults()
{
    auto iconWidget = std::make_unique<KIconWidget>();

    QVERIFY(iconWidget->icon().isNull());
    QCOMPARE(iconWidget->mode(), QIcon::Normal);
    QCOMPARE(iconWidget->state(), QIcon::Off);
}

void KIconWidgetTest::testSize()
{
    auto iconWidget = std::make_unique<KIconWidget>();

    const int styleLargeIconSize = iconWidget->style()->pixelMetric(QStyle::PM_LargeIconSize);
    const QSize largeIconSize{styleLargeIconSize, styleLargeIconSize};

    QCOMPARE(iconWidget->iconSize(), largeIconSize);
    QVERIFY(iconWidget->sizeHint().isEmpty());
    QVERIFY(iconWidget->minimumSizeHint().isEmpty());

    const QIcon invalidIcon = QIcon(QStringLiteral("iconthatreallyshouldntexist.png"));
    QVERIFY(invalidIcon.isNull());
    iconWidget->setIcon(invalidIcon);
    QCOMPARE(iconWidget->iconSize(), largeIconSize);
    QVERIFY(iconWidget->sizeHint().isEmpty());
    QVERIFY(iconWidget->minimumSizeHint().isEmpty());

    const QIcon icon = QIcon(QStringLiteral(":/icons/hint.svg"));
    QVERIFY(!icon.isNull());
    iconWidget->setIcon(icon);

    QCOMPARE(iconWidget->iconSize(), largeIconSize);
    QCOMPARE(iconWidget->sizeHint(), largeIconSize);
    QCOMPARE(iconWidget->minimumSizeHint(), largeIconSize);

    const QSize standardSize{64, 64};
    iconWidget->setIconSize(standardSize);
    QCOMPARE(iconWidget->iconSize(), standardSize);
    QCOMPARE(iconWidget->sizeHint(), standardSize);
    QCOMPARE(iconWidget->minimumSizeHint(), standardSize);

    const QSize weirdSize{13, 37};
    iconWidget->setIconSize(weirdSize);
    QCOMPARE(iconWidget->iconSize(), weirdSize);
    QCOMPARE(iconWidget->sizeHint(), weirdSize);
    QCOMPARE(iconWidget->minimumSizeHint(), weirdSize);

    iconWidget->setIcon(QIcon());
    QCOMPARE(iconWidget->iconSize(), weirdSize);
    QVERIFY(iconWidget->sizeHint().isEmpty());
    QVERIFY(iconWidget->minimumSizeHint().isEmpty());
}

void KIconWidgetTest::testRendering()
{
    auto iconWidget = std::make_unique<KIconWidget>();
    const qreal dpr = iconWidget->devicePixelRatioF();

    const QIcon icon = QIcon(QStringLiteral(":/icons/hint.svg"));
    QVERIFY(!icon.isNull());
    iconWidget->setIcon(icon);

    {
        // Default size should be indistinguishable to an icon rendered at this size.
        const QPixmap iconPixmap = icon.pixmap(iconWidget->iconSize(), dpr);

        QPixmap referencePixmap = createPixmap(iconWidget->iconSize(), dpr);
        QPainter referencePainter{&referencePixmap};
        referencePainter.drawPixmap(0, 0, iconPixmap);

        QPixmap widgetPixmap = createPixmap(iconWidget->iconSize(), dpr);
        QPainter widgetPainter{&widgetPixmap};
        iconWidget->render(&widgetPainter, QPoint(), QRegion(), {} /* render flags, no background */);

        QCOMPARE(widgetPixmap, referencePixmap);
    }

    // Custom icon size.
    const QSize largeSize{128, 128};
    QPixmap iconPixmap = icon.pixmap(largeSize, dpr);
    {
        iconWidget->setIconSize(largeSize);
        iconWidget->adjustSize(); // shouldn't be needed?

        QPixmap referencePixmap = createPixmap(largeSize, dpr);
        QPainter referencePainter{&referencePixmap};
        referencePainter.drawPixmap(0, 0, iconPixmap);

        QPixmap widgetPixmap = createPixmap(iconWidget->iconSize(), dpr);
        QPainter widgetPainter{&widgetPixmap};
        iconWidget->render(&widgetPainter, QPoint(), QRegion(), {} /* render flags, no background */);
        QCOMPARE(widgetPixmap, referencePixmap);
    }

    // Resize widget, icon should be rendered in the center.
    const QSize customSize{500, 400};
    {
        iconWidget->resize(customSize);

        QPixmap referencePixmap = createPixmap(customSize, dpr);
        QPainter referencePainter{&referencePixmap};
        referencePainter.drawPixmap(186, 136, iconPixmap);

        QPixmap widgetPixmap = createPixmap(customSize, dpr);
        QPainter widgetPainter{&widgetPixmap};
        iconWidget->render(&widgetPainter, QPoint(), QRegion(), {} /* render flags, no background */);
        QCOMPARE(widgetPixmap, referencePixmap);
    }
}

void KIconWidgetTest::testMode_data()
{
    QTest::addColumn<QIcon::Mode>("mode");
    QTest::addColumn<QIcon::State>("state");

    // Normal + Off is the default, we tested that already in testRendering.
    QTest::newRow("Disabled, Off") << QIcon::Disabled << QIcon::Off;
    QTest::newRow("Active, Off") << QIcon::Active << QIcon::Off;
    QTest::newRow("Selected, Off") << QIcon::Selected << QIcon::Off;

    QTest::newRow("Normal, On") << QIcon::Normal << QIcon::On;
    QTest::newRow("Disabled, On") << QIcon::Disabled << QIcon::On;
    QTest::newRow("Active, On") << QIcon::Active << QIcon::On;
    QTest::newRow("Selected, On") << QIcon::Selected << QIcon::On;
}

void KIconWidgetTest::testMode()
{
    QFETCH(QIcon::Mode, mode);
    QFETCH(QIcon::State, state);

    auto iconWidget = std::make_unique<KIconWidget>();
    const qreal dpr = iconWidget->devicePixelRatioF();

    const QIcon icon = QIcon(QStringLiteral(":/icons/hint.svg"));
    QVERIFY(!icon.isNull());

    iconWidget->setIcon(icon);
    iconWidget->setMode(mode);
    iconWidget->setState(state);

    QCOMPARE(iconWidget->mode(), mode);
    QCOMPARE(iconWidget->state(), state);

    const QPixmap iconPixmap = icon.pixmap(iconWidget->iconSize(), dpr, mode, state);

    QPixmap referencePixmap = createPixmap(iconWidget->iconSize(), dpr);
    QPainter referencePainter{&referencePixmap};
    referencePainter.drawPixmap(0, 0, iconPixmap);

    QPixmap widgetPixmap = createPixmap(iconWidget->iconSize(), dpr);
    QPainter widgetPainter{&widgetPixmap};
    iconWidget->render(&widgetPainter, QPoint(), QRegion(), {} /* render flags, no background */);

    QCOMPARE(widgetPixmap, referencePixmap);
}

QTEST_MAIN(KIconWidgetTest)

#include "moc_kiconwidgettest.cpp"
