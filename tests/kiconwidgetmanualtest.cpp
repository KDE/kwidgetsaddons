/*
 * SPDX-FileCopyrightText: 2025 Kai Uwe Broulik <kde@broulik.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "kiconwidget.h"

#include <QApplication>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("kiconwidgettext"));

    QDialog dialog;

    auto *layout = new QVBoxLayout(&dialog);
    dialog.setLayout(layout);

    auto *label = new QLabel(u"The icons should be shown at 64px (logical) and update automatically when the color scheme changes."_s, &dialog);
    layout->addWidget(label);

    auto *iconLayout = new QHBoxLayout(&dialog);
    layout->addLayout(iconLayout);

    for (const QString &iconName : {u"dialog-warning"_s, u"user-home"_s, u"audio-volume-muted-symbolic"_s}) {
        KIconWidget *icon = new KIconWidget(QIcon::fromTheme(iconName));
        icon->setIconSize(64);
        iconLayout->addWidget(icon);
    }

    dialog.show();

    return app.exec();
}
