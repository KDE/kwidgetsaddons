/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 2020 David Hurka <david.hurka@mailbox.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include <ktoolbarpopupaction.h>

#include <QTest>
#include <QToolBar>

class KToolBarPopupActionTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 76)
    void testDeprecatedPopupModeApi()
    {
        QToolBar dummyToolbar;
        KToolBarPopupAction menu(QIcon(), QStringLiteral("Menu"), this);

        QCOMPARE(menu.popupMode(), QToolButton::MenuButtonPopup);
        QCOMPARE(menu.delayed(), true);
        QCOMPARE(menu.stickyMenu(), true);

        menu.setDelayed(false);
        QCOMPARE(menu.popupMode(), QToolButton::InstantPopup);
        QCOMPARE(menu.stickyMenu(), true);

        menu.setPopupMode(QToolButton::DelayedPopup);
        QCOMPARE(menu.delayed(), true);
        QCOMPARE(menu.stickyMenu(), false);

        menu.setPopupMode(QToolButton::MenuButtonPopup);
        QCOMPARE(menu.delayed(), true);
        QCOMPARE(menu.stickyMenu(), true);

        menu.setDelayed(true);
        menu.setStickyMenu(false);
        QCOMPARE(menu.popupMode(), QToolButton::DelayedPopup);
        QCOMPARE(menu.delayed(), true);
        QCOMPARE(menu.stickyMenu(), false);

        menu.setDelayed(false);
        QCOMPARE(menu.popupMode(), QToolButton::InstantPopup);

        menu.setStickyMenu(true);
        QCOMPARE(menu.popupMode(), QToolButton::InstantPopup);

        menu.setDelayed(true);
        QCOMPARE(menu.popupMode(), QToolButton::MenuButtonPopup);
    }
#endif
};

QTEST_MAIN(KToolBarPopupActionTest)

#include "ktoolbarpopupactiontest.moc"
