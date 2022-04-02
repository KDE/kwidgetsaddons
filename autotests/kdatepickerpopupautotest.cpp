/*
    SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <KDatePickerPopup>

#include <QSignalSpy>
#include <QTest>

class KDatePickerPopupTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testWordActions()
    {
        KDatePickerPopup p(KDatePickerPopup::DatePicker | KDatePickerPopup::Words | KDatePickerPopup::NoDate);
        // picker, 9 words, no date, 2 separators
        p.popup(QPoint());
        QCOMPARE(p.actions().count(), 13);
        p.hide();
        p.popup(QPoint());
        QCOMPARE(p.actions().count(), 13);

        QCOMPARE(p.actions()[2]->data().toDate(), QDate::currentDate().addYears(1));
        QCOMPARE(p.actions()[6]->data().toDate(), QDate::currentDate());
        QCOMPARE(p.actions()[12]->data().toDate(), QDate());
    }

    void testModes()
    {
        KDatePickerPopup p;
        QCOMPARE(p.modes(), KDatePickerPopup::DatePicker);
        p.popup(QPoint());
        QCOMPARE(p.actions().count(), 1); // only the date picker
        p.hide();

        p.setModes(KDatePickerPopup::NoDate);
        p.popup(QPoint());
        QCOMPARE(p.actions().count(), 1);
        p.hide();

        p.setModes(KDatePickerPopup::Words);
        p.popup(QPoint());
        QCOMPARE(p.actions().count(), 9);
        p.hide();

        p.setModes(KDatePickerPopup::DatePicker | KDatePickerPopup::NoDate);
        p.popup(QPoint());
        QCOMPARE(p.actions().count(), 3);
        p.hide();

        p.setModes(KDatePickerPopup::Words | KDatePickerPopup::NoDate);
        p.popup(QPoint());
        QCOMPARE(p.actions().count(), 11);
        p.hide();
    }
};

QTEST_MAIN(KDatePickerPopupTest)

#include "kdatepickerpopupautotest.moc"
