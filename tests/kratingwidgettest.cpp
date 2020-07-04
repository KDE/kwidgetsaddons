/*
    This file is part of the KDE Libraries
    SPDX-FileCopyrightText: 2013 Kevin Ottens <ervin+bluesystems@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include <QApplication>
#include <QVBoxLayout>

#include <kratingwidget.h>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    QWidget window;
    window.setLayout(new QVBoxLayout);

    KRatingWidget *enabled = new KRatingWidget(&window);
    window.layout()->addWidget(enabled);

    KRatingWidget *disabled = new KRatingWidget(&window);
    disabled->setEnabled(false);
    window.layout()->addWidget(disabled);

    window.show();

    return app.exec();
}

