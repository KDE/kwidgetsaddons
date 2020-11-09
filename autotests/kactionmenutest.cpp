/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 2020 David Hurka <david.hurka@mailbox.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include <kactionmenu.h>

#include <QTest>
#include <QToolBar>

class KActionMenuTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 77)
    void testDeprecatedPopupModeApi()
    {
        QToolBar dummyToolbar;
        KActionMenu menu(QStringLiteral("Menu"), this);

        menu.setDelayed(true);
        menu.setStickyMenu(false);
        QCOMPARE(menu.popupMode(), QToolButton::DelayedPopup);
        QCOMPARE(menu.delayed(), true);
        QCOMPARE(menu.stickyMenu(), false);

        menu.setDelayed(false);
        QCOMPARE(menu.popupMode(), QToolButton::MenuButtonPopup);
        QCOMPARE(menu.stickyMenu(), false);

        menu.setPopupMode(QToolButton::DelayedPopup);
        QCOMPARE(menu.delayed(), true);

        menu.setPopupMode(QToolButton::InstantPopup);
        QCOMPARE(menu.delayed(), false);
        QCOMPARE(menu.stickyMenu(), true);

        menu.setDelayed(true);
        menu.setStickyMenu(true);
        QCOMPARE(menu.popupMode(), QToolButton::DelayedPopup);
        QCOMPARE(menu.stickyMenu(), true);

        menu.setStickyMenu(false);
        QCOMPARE(menu.popupMode(), QToolButton::DelayedPopup);

        menu.setDelayed(false);
        QCOMPARE(menu.popupMode(), QToolButton::MenuButtonPopup);
    }
#endif
};

QTEST_MAIN(KActionMenuTest)

#include "kactionmenutest.moc"
