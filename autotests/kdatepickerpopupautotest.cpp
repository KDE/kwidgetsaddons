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
        // picker, 4 words, no date, 2 separators
        p.popup(QPoint());
        QCOMPARE(p.actions().count(), 8);
        p.hide();
        p.popup(QPoint());
        QCOMPARE(p.actions().count(), 8);
    }
};

QTEST_MAIN(KDatePickerPopupTest)

#include "kdatepickerpopupautotest.moc"
