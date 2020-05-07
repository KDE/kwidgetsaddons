/*
    This file is part of the KDE Libraries

    Copyright (C) 2006 Tobias Koenig (tokoe@kde.org)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB. If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "kpagewidget.h"
#include "kpagewidget_p.h"

#include "kpagewidgetmodel.h"

KPageWidgetPrivate::KPageWidgetPrivate(KPageWidget *q)
    : KPageViewPrivate(q)
{
}

void KPageWidgetPrivate::_k_slotCurrentPageChanged(const QModelIndex &current, const QModelIndex &before)
{
    KPageWidgetItem *currentItem = nullptr;
    if (current.isValid()) {
        currentItem = model()->item(current);
    }

    KPageWidgetItem *beforeItem = nullptr;
    if (before.isValid()) {
        beforeItem = model()->item(before);
    }

    Q_Q(KPageWidget);
    emit q->currentPageChanged(currentItem, beforeItem);
}

KPageWidget::KPageWidget(KPageWidgetPrivate &dd, QWidget *parent)
    : KPageView(dd, parent)
{
    Q_D(KPageWidget);
    connect(this, SIGNAL(currentPageChanged(QModelIndex,QModelIndex)),
            this, SLOT(_k_slotCurrentPageChanged(QModelIndex,QModelIndex)));

    if (!d->KPageViewPrivate::model) {
        setModel(new KPageWidgetModel(this));
    } else {
        Q_ASSERT(qobject_cast<KPageWidgetModel *>(d->KPageViewPrivate::model));
    }

    connect(d->model(), &KPageWidgetModel::toggled,
            this, &KPageWidget::pageToggled);
}

KPageWidget::KPageWidget(QWidget *parent)
    : KPageWidget(*new KPageWidgetPrivate(this), parent)
{
}

KPageWidget::~KPageWidget()
{
}

KPageWidgetItem *KPageWidget::addPage(QWidget *widget, const QString &name)
{
    Q_D(KPageWidget);

    return d->model()->addPage(widget, name);
}

void KPageWidget::addPage(KPageWidgetItem *item)
{
    Q_D(KPageWidget);

    d->model()->addPage(item);
}

KPageWidgetItem *KPageWidget::insertPage(KPageWidgetItem *before, QWidget *widget, const QString &name)
{
    Q_D(KPageWidget);

    return d->model()->insertPage(before, widget, name);
}

void KPageWidget::insertPage(KPageWidgetItem *before, KPageWidgetItem *item)
{
    Q_D(KPageWidget);

    d->model()->insertPage(before, item);
}

KPageWidgetItem *KPageWidget::addSubPage(KPageWidgetItem *parent, QWidget *widget, const QString &name)
{
    Q_D(KPageWidget);

    return d->model()->addSubPage(parent, widget, name);
}

void KPageWidget::addSubPage(KPageWidgetItem *parent, KPageWidgetItem *item)
{
    Q_D(KPageWidget);

    d->model()->addSubPage(parent, item);
}

void KPageWidget::removePage(KPageWidgetItem *item)
{
    Q_D(KPageWidget);

    emit pageRemoved(item); // Q_EMIT signal before we remove it, because the item will be deleted in the model
    d->model()->removePage(item);
}

void KPageWidget::setCurrentPage(KPageWidgetItem *item)
{
    Q_D(KPageWidget);

    const QModelIndex index = d->model()->index(item);
    if (!index.isValid()) {
        return;
    }

    KPageView::setCurrentPage(index);
}

KPageWidgetItem *KPageWidget::currentPage() const
{
    Q_D(const KPageWidget);

    const QModelIndex index = KPageView::currentPage();

    if (!index.isValid()) {
        return nullptr;
    }

    return d->model()->item(index);
}

#include "moc_kpagewidget.cpp"
