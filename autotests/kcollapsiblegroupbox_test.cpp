/* This file is part of the KDE frameworks
 *
 * Copyright (c) 2016 Ragnar Thomsen <rthomsen6@gmail.com>
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

#include "kcollapsiblegroupbox_test.h"

#include <KCollapsibleGroupBox>

#include <QCheckBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(KCollapsibleGroupBoxTest)

void KCollapsibleGroupBoxTest::testDestructorCrash()
{
    // The unittest does not crash the first time,
    // so we run it twice.
    int i = 0;
    while(i != 2) {

        QDialog *dlg = new QDialog(nullptr);

        QVBoxLayout *mainvlayout = new QVBoxLayout(dlg);
        KCollapsibleGroupBox *collapsible = new KCollapsibleGroupBox(dlg);
        collapsible->setTitle(QStringLiteral("A test KCollapsibleGroupBox"));

        QVBoxLayout *collapsiblevlayout = new QVBoxLayout(collapsible);

        QLabel *lbl = new QLabel(QStringLiteral("A test label."), collapsible);
        collapsiblevlayout->addWidget(lbl);

        collapsible->expand();

        mainvlayout->addWidget(collapsible);

        connect(dlg, &QDialog::finished, dlg, &QObject::deleteLater);

        dlg->open();

        QVERIFY(collapsible);

        // If waiting for less than ~500ms the test crashes every time.
        // Not waiting or waiting for more than ~500ms is ok.
        // Using qSleep(400) does NOT provoke the crash.
        QTest::qWait(400);

        dlg->accept();

        i++;
    }
}

void KCollapsibleGroupBoxTest::testOverrideFocus()
{
    KCollapsibleGroupBox collapsible;
    QVBoxLayout layout(&collapsible);

    QCheckBox checkBox;
    QCOMPARE(checkBox.focusPolicy(), Qt::StrongFocus);

    layout.addWidget(&checkBox);
    // The ChildAdded event is processed asynchronously.
    qApp->processEvents();
    // Make sure focus policy has been overridden.
    collapsible.show();
    QVERIFY(checkBox.isVisible());
    QCOMPARE(checkBox.focusPolicy(), Qt::NoFocus);

    // Make sure focus policy is restored on expand event.
    collapsible.expand();
    QVERIFY(checkBox.isVisible());
    QCOMPARE(checkBox.focusPolicy(), Qt::StrongFocus);

    // Make sure focus policy is overridden again on next collapse.
    collapsible.collapse();
    QVERIFY(checkBox.isVisible());
    QCOMPARE(checkBox.focusPolicy(), Qt::NoFocus);
}

void KCollapsibleGroupBoxTest::childShouldGetFocus()
{
    KCollapsibleGroupBox collapsible;
    auto spinBox = new QLineEdit(&collapsible);
    // The ChildAdded event is processed asynchronously.
    // This also "simulates" user who manually expands.
    qApp->processEvents();
    collapsible.expand();
    QCOMPARE(spinBox->focusPolicy(), Qt::StrongFocus);
}
