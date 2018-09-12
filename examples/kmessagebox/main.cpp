/* This file is part of the KDE project
 *
 * Copyright (C) 2018 Friedrich W. H. Kossebau <kossebau@kde.org>
 * Copyright (C) 2018 Olivier Churlaud <olivier@churlaud.com>
 * Copyright (C) 2016 by Juan Carlos Torres <carlosdgtorres@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <QApplication>
#include <KMessageBox>

#include <iostream>

int main (int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Define a button
    KGuiItem yesButton(
        QStringLiteral("Hello"), // the button label
        QStringLiteral("view-filter"), // iconName, can be empty to fallback to default
        QStringLiteral("This is a tooltip"),
        QStringLiteral("This is a WhatsThis help text.")
        );

    KMessageBox::ButtonCode res = KMessageBox::questionYesNo(
        nullptr, // Parent, here none
        // The description can contain HTML
        QStringLiteral("Description to tell you to click<br />on <b>either</b> button"),
        QStringLiteral("My Title"),
        yesButton
        /*noButton not explicitly set, falls back to default*/
    );

    if (res == KMessageBox::Yes) {
        std::cout << "You clicked Hello\n";
        return EXIT_SUCCESS;
    } else {
        std::cout  << "You clicked No\n";
        return EXIT_FAILURE;
    }
}
