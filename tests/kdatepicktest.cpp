/*
 * Copyright 2014 Alex Merry <alex.merry@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "kdatepicker.h"
#include <QApplication>
#include <QTimer>

class Manager : public QObject
{
    Q_OBJECT
public:
    Manager()
    {
        picker.show();
        QTimer::singleShot(1000, this, SLOT(timeout1()));
        QTimer::singleShot(5000, this, SLOT(timeout2()));
    }

private Q_SLOTS:
    void timeout1()
    {
        picker.setEnabled(false);
    }
    void timeout2()
    {
        picker.setEnabled(true);
    }


private:
    KDatePicker picker;
};

int main(int argc, char **argv)
{
    QApplication::setApplicationName(QStringLiteral("KDatePickertest"));
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    QApplication app(argc, argv);

    Manager mgr;

    return app.exec();
}

#include <kdatepicktest.moc>
