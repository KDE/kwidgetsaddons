/*
    Copyright 2011 John Layt <john@layt.net>

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

#include "kdatecomboboxtest.h"

#include <QtCore/QDate>

#include <QtTest/QtTest>
#include <QLineEdit>
#include "kdatecombobox.h"

QTEST_MAIN(KDateComboBoxTest)

void KDateComboBoxTest::testDefaults()
{
    QScopedPointer<KDateComboBox> combo(new KDateComboBox);
    QCOMPARE(combo->date(), QDate::currentDate());
    // Missing support in QLocale;
    //QCOMPARE(m_combo->minimumDate(), KLocale::global()->calendar()->earliestValidDate());
    //QCOMPARE(m_combo->maximumDate(), KLocale::global()->calendar()->latestValidDate());
    QCOMPARE(combo->isValid(), true);
    QCOMPARE(combo->isNull(), false);
    QCOMPARE(combo->options(), KDateComboBox::EditDate | KDateComboBox::SelectDate | KDateComboBox::DatePicker | KDateComboBox::DateKeywords);
    QCOMPARE(combo->displayFormat(), QLocale::ShortFormat);
}

void KDateComboBoxTest::testValidNull()
{
    QScopedPointer<KDateComboBox> combo(new KDateComboBox);
    QCOMPARE(combo->isValid(), true);
    QCOMPARE(combo->isNull(), false);
    combo->setDate(QDate());
    QCOMPARE(combo->isValid(), false);
    QCOMPARE(combo->isNull(), true);
    combo->setDate(QDate(2000, 1, 1));
    combo->lineEdit()->setText(QStringLiteral("invalid"));
    QCOMPARE(combo->isValid(), false);
    QCOMPARE(combo->isNull(), false);
}

void KDateComboBoxTest::testDateRange()
{
    QScopedPointer<KDateComboBox> combo(new KDateComboBox);
    combo->setDate(QDate(2000, 1, 1));
    // Missing support in QLocale;
    //QCOMPARE(m_combo->minimumDate(), KLocale::global()->calendar()->earliestValidDate());
    //QCOMPARE(m_combo->maximumDate(), KLocale::global()->calendar()->latestValidDate());
    QCOMPARE(combo->isValid(), true);

    combo->setDateRange(QDate(2001, 1, 1), QDate(2002, 1, 1));
    QCOMPARE(combo->minimumDate(), QDate(2001, 1, 1));
    QCOMPARE(combo->maximumDate(), QDate(2002, 1, 1));
    QCOMPARE(combo->isValid(), false);

    combo->setDate(QDate(2003, 1, 1));
    QCOMPARE(combo->isValid(), false);
    combo->setDate(QDate(2001, 6, 1));
    QCOMPARE(combo->isValid(), true);
    combo->setDate(QDate(2001, 1, 1));
    QCOMPARE(combo->isValid(), true);
    combo->setDate(QDate(2002, 1, 1));
    QCOMPARE(combo->isValid(), true);
    combo->setDate(QDate(2000, 12, 31));
    QCOMPARE(combo->isValid(), false);
    combo->setDate(QDate(2002, 1, 2));
    QCOMPARE(combo->isValid(), false);

    combo->setDateRange(QDate(1995, 1, 1), QDate(1990, 1, 1));
    QCOMPARE(combo->minimumDate(), QDate(2001, 1, 1));
    QCOMPARE(combo->maximumDate(), QDate(2002, 1, 1));

    combo->setMinimumDate(QDate(2000, 1, 1));
    QCOMPARE(combo->minimumDate(), QDate(2000, 1, 1));
    QCOMPARE(combo->maximumDate(), QDate(2002, 1, 1));

    combo->setMaximumDate(QDate(2003, 1, 1));
    QCOMPARE(combo->minimumDate(), QDate(2000, 1, 1));
    QCOMPARE(combo->maximumDate(), QDate(2003, 1, 1));

    combo->resetDateRange();
    QVERIFY(!combo->minimumDate().isValid());
    QVERIFY(!combo->maximumDate().isValid());

    // Check functioning when the minimum or maximum date is not already set

    combo->setMinimumDate(QDate(2000, 1, 1));
    QCOMPARE(combo->minimumDate(), QDate(2000, 1, 1));
    QVERIFY(!combo->maximumDate().isValid());

    combo->resetMinimumDate();
    QVERIFY(!combo->minimumDate().isValid());
    QVERIFY(!combo->maximumDate().isValid());

    combo->setMaximumDate(QDate(2003, 1, 1));
    QVERIFY(!combo->minimumDate().isValid());
    QCOMPARE(combo->maximumDate(), QDate(2003, 1, 1));

    combo->resetMaximumDate();
    QVERIFY(!combo->minimumDate().isValid());
    QVERIFY(!combo->maximumDate().isValid());

}

void KDateComboBoxTest::testDateList()
{
    QScopedPointer<KDateComboBox> combo(new KDateComboBox);
    QMap<QDate, QString> map;

    // Test default map
    QCOMPARE(combo->dateMap(), map);

    // Test basic map
    map.clear();
    map.insert(QDate(2000, 1, 1), QStringLiteral("New Years Day"));
    map.insert(QDate(2000, 1, 2), QString());
    map.insert(QDate(2000, 1, 3), QStringLiteral("separator"));
    map.insert(QDate(), QStringLiteral("No Date"));
    combo->setDateMap(map);
    QCOMPARE(combo->dateMap(), map);
}

void KDateComboBoxTest::testOptions()
{
    QScopedPointer<KDateComboBox> combo(new KDateComboBox);
    KDateComboBox::Options options = KDateComboBox::EditDate | KDateComboBox::SelectDate | KDateComboBox::DatePicker | KDateComboBox::DateKeywords;
    QCOMPARE(combo->options(), options);
    options = KDateComboBox::EditDate | KDateComboBox::WarnOnInvalid;
    combo->setOptions(options);
    QCOMPARE(combo->options(), options);
}

void KDateComboBoxTest::testDisplayFormat()
{
    QScopedPointer<KDateComboBox> combo(new KDateComboBox);
    QLocale::FormatType format = QLocale::ShortFormat;
    QCOMPARE(combo->displayFormat(), format);
    format = QLocale::NarrowFormat;
    combo->setDisplayFormat(format);
    QCOMPARE(combo->displayFormat(), format);
}
