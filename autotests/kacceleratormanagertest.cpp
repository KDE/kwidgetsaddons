/* This file is part of the KDE libraries
 *
 * Copyright (c) 2017 David Faure <faure@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */
#include <kacceleratormanager.h>

#include <QTest>
#include <QMenu>

#define QSL QStringLiteral

static QStringList extractActionTexts(QMenu &menu, std::function<QString(const QAction*)> func)
{
    menu.aboutToShow(); // signals are now public in Qt5, how convenient :-)

    QStringList ret;
    bool lastIsSeparator = false;
    foreach (const QAction *action, menu.actions()) {
        if (action->isSeparator()) {
            if (!lastIsSeparator) { // Qt gets rid of duplicate separators, so we should too
                ret.append(QStringLiteral("separator"));
            }
            lastIsSeparator = true;
        } else {
            lastIsSeparator = false;
            if (action->menu()) {
                ret.append(QStringLiteral("submenu"));
            } else {
                const QString text = func(action);
                ret.append(text);
            }
        }
    }
    return ret;
}

class KAcceleratorManagerTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase()
    {
    }

    void testActionTexts_data()
    {
        QTest::addColumn<QStringList>("initialTexts");
        QTest::addColumn<QStringList>("expectedTexts");

        QTest::newRow("basic") << QStringList{QSL("Open"), QSL("Close"), QSL("Quit")} << QStringList{QSL("&Open"), QSL("&Close"), QSL("&Quit")};
        QTest::newRow("same_first") << QStringList{QSL("Open"), QSL("Close"), QSL("Clone"), QSL("Quit")} << QStringList{QSL("&Open"), QSL("&Close"), QSL("C&lone"), QSL("&Quit")};
        QTest::newRow("cjk") << QStringList{QSL("Open (&O)"), QSL("Close (&C)"), QSL("Clone (&C)"), QSL("Quit (&Q)")} << QStringList{QSL("Open (&O)"), QSL("Close (&C)"), QSL("C&lone (C)"), QSL("Quit (&Q)")};
    }

    void testActionTexts()
    {
        // GIVEN
        QFETCH(QStringList, initialTexts);
        QFETCH(QStringList, expectedTexts);
        QMenu menu;
        for (const QString &text : qAsConst(initialTexts)) {
            menu.addAction(text);
        }
        // WHEN
        KAcceleratorManager::manage(&menu);
        // THEN
        const QStringList texts = extractActionTexts(menu, &QAction::text);
        QCOMPARE(texts, expectedTexts);
    }

    void testActionIconTexts_data()
    {
        QTest::addColumn<QStringList>("initialTexts");
        QTest::addColumn<QStringList>("expectedTexts");

        QTest::newRow("basic") << QStringList{QSL("Open"), QSL("Close"), QSL("Quit")} << QStringList{QSL("Open"), QSL("Close"), QSL("Quit")};
        QTest::newRow("same_first") << QStringList{QSL("Open"), QSL("Close"), QSL("Clone"), QSL("Quit")} << QStringList{QSL("Open"), QSL("Close"), QSL("Clone"), QSL("Quit")};
        QTest::newRow("cjk") << QStringList{QSL("Open (&O)"), QSL("Close (&C)"), QSL("Clone (&C)"), QSL("Quit (&Q)")} << QStringList{QSL("Open"), QSL("Close"), QSL("Clone"), QSL("Quit")};
    }

    void testActionIconTexts()
    {
        // GIVEN
        QFETCH(QStringList, initialTexts);
        QFETCH(QStringList, expectedTexts);
        QMenu menu;
        for (const QString &text : qAsConst(initialTexts)) {
            menu.addAction(text);
        }
        // WHEN
        KAcceleratorManager::manage(&menu);
        // THEN
        const QStringList iconTexts = extractActionTexts(menu, &QAction::iconText);
        QCOMPARE(iconTexts, expectedTexts);
    }
};

QTEST_MAIN(KAcceleratorManagerTest)

#include "kacceleratormanagertest.moc"
