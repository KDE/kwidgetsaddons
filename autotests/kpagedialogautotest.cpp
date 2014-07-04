/*
    Copyright 2014 Laurent Montel <montel@kde.org>

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

#include "kpagedialogautotest.h"
#include <kpagedialog.h>

#include <QDialogButtonBox>
#include <QPushButton>
#include <QtTest/QtTest>

QTEST_MAIN(KPageDialogAutoTest)

KPageDialogAutoTest::KPageDialogAutoTest()
{
}

void KPageDialogAutoTest::shouldHaveDefaultValuesOnCreation()
{
    KPageDialog page;
    QDialogButtonBox *dialogbuttonbox = page.findChild<QDialogButtonBox *>(QLatin1String("buttonbox"));
    QVERIFY(dialogbuttonbox);
    QDialogButtonBox::StandardButtons standardButton = dialogbuttonbox->standardButtons();
    QDialogButtonBox::StandardButtons defaultButton = QDialogButtonBox::Ok | QDialogButtonBox::Cancel;
    QCOMPARE(standardButton, defaultButton);
}

void KPageDialogAutoTest::shouldAddAnActionButton()
{
    KPageDialog page;
    QDialogButtonBox *dialogbuttonbox = page.findChild<QDialogButtonBox *>(QLatin1String("buttonbox"));
    QPushButton *actionButton = new QPushButton(QLatin1String("Action1"));
    page.addActionButton(actionButton);
    QCOMPARE(dialogbuttonbox->buttons().count(), 3);
    QVERIFY(dialogbuttonbox->buttons().contains(actionButton));
}

void KPageDialogAutoTest::shouldAddTwoActionButton()
{
    KPageDialog page;
    QDialogButtonBox *dialogbuttonbox = page.findChild<QDialogButtonBox *>(QLatin1String("buttonbox"));
    QPushButton *actionButton = new QPushButton(QLatin1String("Action1"));
    page.addActionButton(actionButton);

    QPushButton *actionButton2 = new QPushButton(QLatin1String("Action2"));
    page.addActionButton(actionButton2);

    QCOMPARE(dialogbuttonbox->buttons().count(), 4);
}

void KPageDialogAutoTest::shouldNotAddTwoSameActionButton()
{
    KPageDialog page;
    QDialogButtonBox *dialogbuttonbox = page.findChild<QDialogButtonBox *>(QLatin1String("buttonbox"));
    QPushButton *actionButton = new QPushButton(QLatin1String("Action1"));
    page.addActionButton(actionButton);

    page.addActionButton(actionButton);
    QCOMPARE(dialogbuttonbox->buttons().count(), 3);
}

