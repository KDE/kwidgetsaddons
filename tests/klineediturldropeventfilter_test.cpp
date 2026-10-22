/*
 * SPDX-FileCopyrightText: 2026 Kai Uwe Broulik <kde@broulik.de>
 *
 * SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QApplication>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>

#include <KLineEditUrlDropEventFilter>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget *w = new QWidget;
    QVBoxLayout *vbox = new QVBoxLayout(w);

    auto *lineEdit = new QLineEdit(w);
    lineEdit->setPlaceholderText(QStringLiteral("Drop file or URL here…"));
    lineEdit->installEventFilter(new KLineEditUrlDropEventFilter(lineEdit));
    vbox->addWidget(lineEdit);
    vbox->addStretch();

    w->resize(400, 400);
    w->show();

    return app.exec();
}
