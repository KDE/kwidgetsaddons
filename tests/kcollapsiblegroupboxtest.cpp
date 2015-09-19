/* This file is part of the KDE libraries
 *
 * Copyright 2015 David Edmundson <davidedmundson@kde.org>
 *
 * Based on test program by Dominik Haumann <dhaumann@kde.org>
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
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QFrame>
#include <QCheckBox>
#include <QGroupBox>
#include <QLabel>

#include <kcollapsiblegroupbox.h>
#include "kdatepicker.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    QWidget mainWindow;

    QVBoxLayout mainWindowLayout(&mainWindow);

    //add a layout with many items to make sure it doesn't shuffle them weirdly when animating
    {
        auto groupBox = new KCollapsibleGroupBox;
        groupBox->setTitle(QStringLiteral("&Advanced Options"));

        auto innerLayout = new QVBoxLayout;
        for (int i =0; i<7;i++) {
            auto checkBox = new QCheckBox(QStringLiteral("Some text"));
            innerLayout->addWidget(checkBox);
        }
        groupBox->setLayout(innerLayout);
        mainWindowLayout.addWidget(groupBox);
    }

    //another item which should expand to fill width
    {
        auto groupBox = new KCollapsibleGroupBox;
        groupBox->setTitle(QStringLiteral("Pick a &date"));

        auto innerLayout = new QVBoxLayout;
        auto datePicker = new KDatePicker();
        innerLayout->addWidget(datePicker);
        groupBox->setLayout(innerLayout);
        mainWindowLayout.addWidget(groupBox);
    }

    mainWindowLayout.addStretch();
    mainWindow.setLayout(&mainWindowLayout);

    mainWindow.resize(400, 300);
    mainWindow.show();

    return app.exec();
}

// kate: replace-tabs on;
