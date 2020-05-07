/*
 *  This file is part of the KDE Libraries
 *  Copyright (C) 1999-2001 Mirko Boehm (mirko@kde.org) and
 *  Espen Sand (espen@kde.org)
 *  Holger Freyther <freyther@kde.org>
 *  2005-2006 Olivier Goffart <ogoffart at kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#include "kpagedialog.h"
#include "kpagedialog_p.h"

#include <QLayout>
#include <QStyle>

KPageDialog::KPageDialog(QWidget *parent, Qt::WindowFlags flags)
    : KPageDialog(*new KPageDialogPrivate(this), nullptr, parent, flags)
{
}

KPageDialog::KPageDialog(KPageWidget *widget, QWidget *parent, Qt::WindowFlags flags)
    : KPageDialog(*new KPageDialogPrivate(this), widget, parent, flags)
{
    Q_ASSERT(widget);
}

KPageDialog::KPageDialog(KPageDialogPrivate &dd, KPageWidget *widget, QWidget *parent, Qt::WindowFlags flags)
    : QDialog(parent, flags),
      d_ptr(&dd)
{
    Q_D(KPageDialog);
    if (widget) {
        widget->setParent(this);
        d->mPageWidget = widget;
    } else {
        d->mPageWidget = new KPageWidget(this);
    }
    d->mButtonBox = new QDialogButtonBox(this);
    d->mButtonBox->setObjectName(QStringLiteral("buttonbox"));
    d->mButtonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    d->init();
}

KPageDialog::~KPageDialog()
{
    delete d_ptr;
}

void KPageDialog::setFaceType(FaceType faceType)
{
    Q_D(KPageDialog);

    d->mPageWidget->setFaceType(static_cast<KPageWidget::FaceType>(faceType));

    // Use zero margins for dialogs with the sidebar style so that the sidebar
    // can be flush with the window edge; margins for the content are added
    // automatically
    if (faceType == KPageDialog::Auto || faceType == KPageDialog::List){
        layout()->setContentsMargins(0, 0, 0, 0);
    } else {
        const QStyle *style = d->mPageWidget->style();
        layout()->setContentsMargins(
            style->pixelMetric(QStyle::PM_LayoutLeftMargin),
            style->pixelMetric(QStyle::PM_LayoutTopMargin),
            style->pixelMetric(QStyle::PM_LayoutRightMargin),
            style->pixelMetric(QStyle::PM_LayoutBottomMargin)
        );
    }
}

KPageWidgetItem *KPageDialog::addPage(QWidget *widget, const QString &name)
{
    Q_D(KPageDialog);

    return d->mPageWidget->addPage(widget, name);
}

void KPageDialog::addPage(KPageWidgetItem *item)
{
    Q_D(KPageDialog);

    d->mPageWidget->addPage(item);
}

KPageWidgetItem *KPageDialog::insertPage(KPageWidgetItem *before, QWidget *widget, const QString &name)
{
    Q_D(KPageDialog);

    return d->mPageWidget->insertPage(before, widget, name);
}

void KPageDialog::insertPage(KPageWidgetItem *before, KPageWidgetItem *item)
{
    Q_D(KPageDialog);

    d->mPageWidget->insertPage(before, item);
}

KPageWidgetItem *KPageDialog::addSubPage(KPageWidgetItem *parent, QWidget *widget, const QString &name)
{
    Q_D(KPageDialog);

    return d->mPageWidget->addSubPage(parent, widget, name);
}

void KPageDialog::addSubPage(KPageWidgetItem *parent, KPageWidgetItem *item)
{
    Q_D(KPageDialog);

    d->mPageWidget->addSubPage(parent, item);
}

void KPageDialog::removePage(KPageWidgetItem *item)
{
    Q_D(KPageDialog);

    d->mPageWidget->removePage(item);
}

void KPageDialog::setCurrentPage(KPageWidgetItem *item)
{
    Q_D(KPageDialog);

    d->mPageWidget->setCurrentPage(item);
}

KPageWidgetItem *KPageDialog::currentPage() const
{
    Q_D(const KPageDialog);

    return d->mPageWidget->currentPage();
}

void KPageDialog::setStandardButtons(QDialogButtonBox::StandardButtons buttons)
{
    Q_D(KPageDialog);

    d->mButtonBox->setStandardButtons(buttons);
}

QPushButton *KPageDialog::button(QDialogButtonBox::StandardButton which) const
{
    Q_D(const KPageDialog);

    return d->mButtonBox->button(which);
}

void KPageDialog::addActionButton(QAbstractButton *button)
{
    Q_D(KPageDialog);

    d->mButtonBox->addButton(button, QDialogButtonBox::ActionRole);
}

KPageWidget *KPageDialog::pageWidget()
{
    Q_D(KPageDialog);

    return d->mPageWidget;
}

void KPageDialog::setPageWidget(KPageWidget *widget)
{
    Q_D(KPageDialog);

    delete d->mPageWidget;
    d->mPageWidget = widget;
    d->init();
}

const KPageWidget *KPageDialog::pageWidget() const
{
    Q_D(const KPageDialog);

    return d->mPageWidget;
}

QDialogButtonBox *KPageDialog::buttonBox()
{
    Q_D(KPageDialog);

    return d->mButtonBox;
}

const QDialogButtonBox *KPageDialog::buttonBox() const
{
    Q_D(const KPageDialog);

    return d->mButtonBox;
}

void KPageDialog::setButtonBox(QDialogButtonBox *box)
{
    Q_D(KPageDialog);

    delete d->mButtonBox;
    d->mButtonBox = box;
    d->init();
}

