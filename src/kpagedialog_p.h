/*  This file is part of the KDE project
    Copyright (C) 2007 Matthias Kretz <kretz@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2 as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.

*/

#ifndef PAGED_KPAGEDIALOG_P_H
#define PAGED_KPAGEDIALOG_P_H

#include "kpagedialog.h"
#include <QDialogButtonBox>
#include <QLayout>
#include <QAbstractItemView>

class KPageDialogPrivate
{
    Q_DECLARE_PUBLIC(KPageDialog)
protected:
    KPageDialogPrivate(KPageDialog *parent)
        : q_ptr(parent),
          mPageWidget(nullptr),
          mButtonBox(nullptr)
    {
    }

    virtual ~KPageDialogPrivate()
    {
    }

    KPageDialog *const q_ptr;
    KPageWidget *mPageWidget;
    QDialogButtonBox *mButtonBox;

    void init()
    {
        Q_Q(KPageDialog);
        delete q->layout();

        QVBoxLayout *layout = new QVBoxLayout;
        q->setLayout(layout);
        layout->setContentsMargins(0, 0, 0, 0);

        if (mPageWidget) {
            q->connect(mPageWidget, &KPageWidget::currentPageChanged,
                       q, &KPageDialog::currentPageChanged);
            q->connect(mPageWidget, &KPageWidget::pageRemoved,
                       q, &KPageDialog::pageRemoved);
            layout->addWidget(mPageWidget);
        } else {
            layout->addStretch();
        }

        if (mButtonBox) {
            q->connect(mButtonBox, &QDialogButtonBox::accepted, q, &QDialog::accept);
            q->connect(mButtonBox, &QDialogButtonBox::rejected, q, &QDialog::reject);
            if (mPageWidget) {
                mPageWidget->setPageFooter(mButtonBox);
            } else {
                layout->addWidget(mButtonBox);
            }
        }
    }
};

#endif // PAGED_KPAGEDIALOG_P_H
