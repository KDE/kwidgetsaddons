/*
Copyright (C) 1996 Bernd Johannes Wuebben  <wuebben@kde.org>
Copyright (c) 1999 Preston Brown <pbrown@kde.org>
Copyright (c) 1999 Mario Weilguni <mweilguni@kde.org>

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

#include "kfontchooserdialog.h"

#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

class KFontChooserDialogPrivate
{
public:
    KFontChooser *chooser = nullptr;
};

KFontChooserDialog::KFontChooserDialog(const KFontChooser::DisplayFlags &flags, QWidget *parent)
    : QDialog(parent),
      d(new KFontChooserDialogPrivate)
{
    setWindowTitle(tr("Select Font", "@title:window"));
    d->chooser = new KFontChooser(this, flags, QStringList(), 8, nullptr);
    d->chooser->setObjectName(QStringLiteral("fontChooser"));

    connect(d->chooser, &KFontChooser::fontSelected, this, &KFontChooserDialog::fontSelected);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    mainLayout->addWidget(d->chooser);
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

KFontChooserDialog::~KFontChooserDialog()
{
    delete d;
}

void KFontChooserDialog::setFont(const QFont &font, bool onlyFixed)
{
    d->chooser->setFont(font, onlyFixed);
}

QFont KFontChooserDialog::font() const
{
    return d->chooser->font();
}

// If the styleName property is set for a QFont, using setBold(true) would
// lead to Qt using an "emboldended"/synthetic font style instead of using
// the bold style provided by the font itself; the latter looks better than
// the former, also accorgin to upstream, the styleName property is useful
// for fancy font styles, so there is no point in setting it for "Regular"
// fonts. For more details see:
// https://bugreports.qt.io/browse/QTBUG-63792
// https://bugs.kde.org/show_bug.cgi?id=378523
static void stripRegularStyleName(QFont &font)
{
    if (font.weight() == QFont::Normal
        && (font.styleName() == QLatin1String("Regular")
            || font.styleName() == QLatin1String("Normal")
            || font.styleName() == QLatin1String("Book")
            || font.styleName() == QLatin1String("Roman"))) {
        font.setStyleName(QString());
    }
}

// static
int KFontChooserDialog::getFontDiff(QFont &theFont, KFontChooser::FontDiffFlags &diffFlags,
                             const KFontChooser::DisplayFlags &flags, QWidget *parent)
{
    KFontChooserDialog dlg(flags | KFontChooser::ShowDifferences, parent);
    dlg.setObjectName(QStringLiteral("Font Selector"));
    dlg.setFont(theFont, flags & KFontChooser::FixedFontsOnly);

    const int result = dlg.exec();
    if (result == Accepted) {
        theFont = dlg.d->chooser->font();
        diffFlags = dlg.d->chooser->fontDiffFlags();
        stripRegularStyleName(theFont);
    }
    return result;
}

// static
int KFontChooserDialog::getFont(QFont &theFont, const KFontChooser::DisplayFlags &flags, QWidget *parent)
{
    KFontChooserDialog dlg(flags, parent);
    dlg.setObjectName(QStringLiteral("Font Selector"));
    dlg.setFont(theFont, flags & KFontChooser::FixedFontsOnly);

    const int result = dlg.exec();
    if (result == Accepted) {
        theFont = dlg.d->chooser->font();
        stripRegularStyleName(theFont);
    }
    return result;
}
