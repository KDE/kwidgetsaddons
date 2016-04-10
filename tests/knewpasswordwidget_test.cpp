/*
    Copyright (c) 2016 Elvis Angelaccio <elvis.angelaccio@kdemail.net>

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

#include <QApplication>
#include <QCheckBox>
#include <QVBoxLayout>

#include <kcollapsiblegroupbox.h>
#include <knewpasswordwidget.h>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    QWidget mainWindow;
    QVBoxLayout mainWindowLayout(&mainWindow);

    KCollapsibleGroupBox collapsible(&mainWindow);
    collapsible.setExpanded(true);
    collapsible.setTitle(QStringLiteral("A collapsible groupbox"));

    KNewPasswordWidget pwdWidget(&collapsible);
    pwdWidget.setPasswordStrengthMeterVisible(false);

    QCheckBox checkbox(QStringLiteral("A checkbox"), &collapsible);

    QVBoxLayout *layout = new QVBoxLayout(&collapsible);
    layout->addWidget(&pwdWidget);
    layout->addWidget(&checkbox);

    mainWindowLayout.addWidget(&collapsible);
    mainWindow.setLayout(&mainWindowLayout);
    mainWindow.show();

    return app.exec();
}

