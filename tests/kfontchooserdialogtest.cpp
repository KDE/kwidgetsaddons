/*
    Copyright (C) 1996 Bernd Johannes Wuebben
                       wuebben@math.cornell.edu

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/

#include "kfontchooserdialog.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>

int main(int argc, char **argv)
{
    QApplication::setApplicationName(QStringLiteral("KFontChooserDialogTest"));

    QApplication app(argc, argv);

    app.setFont(QFont(QStringLiteral("Noto Sans"), 11));

    QCommandLineParser parser;
    QCommandLineOption syncOption(QStringList{QStringLiteral("sync")}, QStringLiteral("Do sync calls"));
    parser.addOption(syncOption);
    parser.addHelpOption();
    parser.process(app);

    int nRet;
    const bool doSync = parser.isSet(syncOption);
    //  QFont font = QFont("Times",18,QFont::Bold);

    if (doSync) {
        qDebug() << "Sync calls:";

        QFont font;
        qDebug() << "Default use case, all bells and whistles";
        nRet = KFontChooserDialog::getFont(font);
        qDebug() << font.toString();

        qDebug() << "Only show monospaced fonts, FixedOnly checkbox is _not_ shown";
        nRet = KFontChooserDialog::getFont(font, KFontChooser::FixedFontsOnly);
        qDebug() << font.toString();

        KFontChooser::FontDiffFlags diffFlags;
        qDebug() << "ShowDifferences mode";
        nRet = KFontChooserDialog::getFontDiff(font, diffFlags);
        qDebug() << font.toString();

        qDebug() << "ShowDifferences mode and only showing monospaced fonts (the FixedOnly checkbox is _not_ shown)";
        nRet = KFontChooserDialog::getFontDiff(font, diffFlags, KFontChooser::FixedFontsOnly);
        qDebug() << font.toString();
    } else {
        qDebug() << "Async calls:";

        qDebug() << "Default use case, all bells and whistles";
        QFont font;
        KFontChooserDialog::getFont(font, nullptr, &app, [&](const QFont &selectedFont, int dialogCode) {
            nRet = dialogCode;
            if (dialogCode == QDialog::Accepted) {
                font = selectedFont;
            }
            qDebug() << font.toString();

            qDebug() << "Only show monospaced fonts, FixedOnly checkbox is _not_ shown";
            KFontChooserDialog::getFont(font, KFontChooser::FixedFontsOnly, nullptr, &app, [&](const QFont &selectedFont, int dialogCode) {
                nRet = dialogCode;
                if (dialogCode == QDialog::Accepted) {
                    font = selectedFont;
                }
                qDebug() << font.toString();

                qDebug() << "ShowDifferences mode";
                KFontChooserDialog::getFontDiff(font, nullptr, &app,
                                                    [&](const QFont &selectedFont, KFontChooser::FontDiffFlags diffFlags, int dialogCode) {
                    nRet = dialogCode;
                    if (dialogCode == QDialog::Accepted) {
                        font = selectedFont;
                    }
                    qDebug() << font.toString() << diffFlags;

                    qDebug() << "ShowDifferences mode and only showing monospaced fonts (the FixedOnly checkbox is _not_ shown)";
                    KFontChooserDialog::getFontDiff(font, KFontChooser::FixedFontsOnly, nullptr, &app,
                                                    [&](const QFont &selectedFont, KFontChooser::FontDiffFlags diffFlags, int dialogCode) {
                        nRet = dialogCode;
                        if (dialogCode == QDialog::Accepted) {
                            font = selectedFont;
                        }
                        qDebug() << font.toString() << diffFlags;
                    });
                });
            });
        });

        app.exec();
    }

    return nRet;
}
