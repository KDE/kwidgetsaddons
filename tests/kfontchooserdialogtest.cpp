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
#include <QDebug>

int main(int argc, char **argv)
{
    QApplication::setApplicationName(QStringLiteral("KFontChooserDialogTest"));

    QApplication app(argc, argv);

    app.setFont(QFont(QStringLiteral("Noto Sans"), 11));

    //  QFont font = QFont("Times",18,QFont::Bold);

    QFont font;
    qDebug() << "Default use case, all bells and whistles";
    int nRet = KFontChooserDialog::getFont(font);
    qDebug() << font.toString();

    qDebug() << "Only show monospaced fonts, FixedOnly checkbox is _not_ shown";
    nRet = KFontChooserDialog::getFont(font, KFontChooser::FixedFontsOnly);
    qDebug() << font.toString();

    qDebug() << "Don't show the FixedOnly checkbox";
    nRet = KFontChooserDialog::getFont(font, KFontChooser::NoFixedCheckBox);
    qDebug() << font.toString();

    KFontChooser::FontDiffFlags diffFlags;
    qDebug() << "ShowDifferences mode";
    nRet = KFontChooserDialog::getFontDiff(font, diffFlags);
    qDebug() << font.toString();

    qDebug() << "ShowDifferences mode and only showing monospaced fonts (the FixedOnly checkbox is _not_ shown)";
    nRet = KFontChooserDialog::getFontDiff(font, diffFlags, KFontChooser::FixedFontsOnly);
    qDebug() << font.toString();

    qDebug() << "ShowDifferences mode with FixeOnly checkbox not shown";
    nRet = KFontChooserDialog::getFontDiff(font, diffFlags, KFontChooser::NoFixedCheckBox);
    qDebug() << font.toString();

    return nRet;
}
