/*
    Copyright 2019 Harald Sitter <sitter@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "kbusyindicatorwidget.h"

#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QWidget window;
    window.setBaseSize(128, 128);
    auto layout = new QVBoxLayout(&window);

    auto busyWidget = new QWidget(&window);
    auto busyLayout = new QHBoxLayout(busyWidget);
    auto busyIndicator = new KBusyIndicatorWidget(&window);
    auto busyLabel = new QLabel(QStringLiteral("Busy..."), &window);
    busyLayout->addWidget(busyIndicator);
    busyLayout->addWidget(busyLabel);

    auto toggle = new QPushButton(QStringLiteral("Toggle Visible"), &window);

    QObject::connect(toggle, &QPushButton::clicked,
            busyWidget, [=] {
        busyWidget->setVisible(!busyWidget->isVisible());
    });

    layout->addWidget(toggle);
    layout->addWidget(busyWidget);
    layout->setAlignment(Qt::AlignTop);

    window.show();
    return app.exec();
}
