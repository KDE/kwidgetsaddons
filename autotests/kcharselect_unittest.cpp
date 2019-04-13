/* This file is part of the KDE libraries
 *
 * Copyright (c) 2019 David Faure <faure@kde.org>
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
#include <kcharselect.h>

#include <QComboBox>
#include <QTest>

class KCharSelectTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase()
    {
    }

    void createInstance()
    {
        KCharSelect selector(nullptr, nullptr);
        QCOMPARE(selector.displayedCodePoints().count(), 128);
        QCOMPARE(selector.currentCodePoint(), 0);
    }

    void changeBlock()
    {
        KCharSelect selector(nullptr, nullptr);
        // This asserts in QHeaderView, fix is up at https://codereview.qt-project.org/258967
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
        QComboBox *blockCombo = selector.findChild<QComboBox *>(QStringLiteral("blockCombo"));
        QVERIFY(blockCombo);
        blockCombo->setCurrentIndex(1);
        QCOMPARE(selector.currentCodePoint(), 128);
#endif
    }
};

QTEST_MAIN(KCharSelectTest)

#include "kcharselect_unittest.moc"
