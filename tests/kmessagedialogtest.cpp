/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 2020 Ahmad Samir <a.samirh78@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "kmessagedialog.h"

#include <KGuiItem>
#include <KStandardGuiItem>

#include <QApplication>
#include <QDebug>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>

#include <memory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    app.setApplicationName(QStringLiteral("kmessagedialogtest"));

    const auto types = {KMessageDialog::QuestionYesNo,
                        KMessageDialog::QuestionYesNoCancel,
                        KMessageDialog::WarningYesNo,
                        KMessageDialog::WarningYesNoCancel,
                        KMessageDialog::WarningContinueCancel,
                        KMessageDialog::Information,
                        KMessageDialog::Sorry,
                        KMessageDialog::Error};

    for (auto type : types) {
        auto dlg = std::unique_ptr<KMessageDialog>(new KMessageDialog(type, QStringLiteral("Do you agree to action foo?"), nullptr));
        dlg->setCaption(QString{});
        dlg->setIcon(QIcon{});

        auto getResult = [&]() {
            const auto result = static_cast<QDialogButtonBox::StandardButton>(dlg->exec());
            switch (result) {
            case QDialogButtonBox::Ok:
            case QDialogButtonBox::Yes:
                qDebug() << "Button OK/Yes/Continue clicked."
                         << "Don't ask again status:" << (dlg->isDontAskAgainChecked() ? "Checked" : "Unchecked");
                break;
            case QDialogButtonBox::No:
                qDebug() << "Button No clicked"
                         << "Don't ask again status:" << (dlg->isDontAskAgainChecked() ? "Checked" : "Unchecked");
                break;
            case QDialogButtonBox::Cancel:
                qDebug() << "Button Cancel clicked";
                break;
            default:
                break;
            }
        };

        getResult();

        dlg->setDontAskAgainText(QStringLiteral("Do not show this again"));
        dlg->setDontAskAgainChecked(false);
        getResult();

        dlg->setListWidgetItems(QStringList{QStringLiteral("file1"), QStringLiteral("file2")});
        getResult();

        dlg->setDetails(QStringLiteral("Some more details."));
        getResult();
    }
}
