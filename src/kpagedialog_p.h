/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2007 Matthias Kretz <kretz@kde.org>

    SPDX-License-Identifier: LGPL-2.0-only
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
