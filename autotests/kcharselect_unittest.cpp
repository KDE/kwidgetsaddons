/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 2019 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
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
        QComboBox *blockCombo = selector.findChild<QComboBox *>(QStringLiteral("blockCombo"));
        QVERIFY(blockCombo);
        blockCombo->setCurrentIndex(1);
        QCOMPARE(selector.currentCodePoint(), 128);
    }
};

QTEST_MAIN(KCharSelectTest)

#include "kcharselect_unittest.moc"
