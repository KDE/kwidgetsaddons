/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>
    SPDX-FileContributor: Stephen Kelly <stephen@kdab.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kviewstatemaintainerbase.h"

#include <QPointer>
#include <QItemSelectionModel>
#include <QAbstractItemView>

class KViewStateMaintainerBasePrivate
{
    KViewStateMaintainerBasePrivate(KViewStateMaintainerBase *qq)
        : q_ptr(qq)
    {

    }

    Q_DECLARE_PUBLIC(KViewStateMaintainerBase)
    KViewStateMaintainerBase *const q_ptr;

    void _k_modelAboutToBeReset();
    void _k_modelReset();

    QPointer<QAbstractItemView> m_view;
    QPointer<QItemSelectionModel> m_selectionModel;
};

void KViewStateMaintainerBasePrivate::_k_modelAboutToBeReset()
{
    Q_Q(KViewStateMaintainerBase);
    q->saveState();
}

void KViewStateMaintainerBasePrivate::_k_modelReset()
{
    Q_Q(KViewStateMaintainerBase);
    q->restoreState();
}

KViewStateMaintainerBase::KViewStateMaintainerBase(QObject *parent)
    : QObject(parent), d_ptr(new KViewStateMaintainerBasePrivate(this))
{

}

KViewStateMaintainerBase::~KViewStateMaintainerBase() = default;

QItemSelectionModel *KViewStateMaintainerBase::selectionModel() const
{
    Q_D(const KViewStateMaintainerBase);
    if (d->m_selectionModel.isNull()) {
        return nullptr;
    }
    return d->m_selectionModel.data();
}

void KViewStateMaintainerBase::setSelectionModel(QItemSelectionModel *selectionModel)
{
    Q_D(KViewStateMaintainerBase);
    d->m_selectionModel = selectionModel;

    if (d->m_view && d->m_view.data()->model()) {
        disconnect(d->m_view.data()->model(), SIGNAL(modelAboutToBeReset()), this, SLOT(_k_modelAboutToBeReset()));
        disconnect(d->m_view.data()->model(), SIGNAL(modelReset()), this, SLOT(_k_modelReset()));
    }
    connect(d->m_selectionModel.data()->model(), SIGNAL(modelAboutToBeReset()), SLOT(_k_modelAboutToBeReset()), Qt::UniqueConnection);
    connect(d->m_selectionModel.data()->model(), SIGNAL(modelReset()), SLOT(_k_modelReset()), Qt::UniqueConnection);
}

QAbstractItemView *KViewStateMaintainerBase::view() const
{
    Q_D(const KViewStateMaintainerBase);
    if (d->m_view.isNull()) {
        return nullptr;
    }
    return d->m_view.data();
}

void KViewStateMaintainerBase::setView(QAbstractItemView *view)
{
    Q_D(KViewStateMaintainerBase);
    d->m_view = view;

    if (d->m_selectionModel && d->m_selectionModel.data()->model()) {
        disconnect(d->m_selectionModel.data()->model(), SIGNAL(modelAboutToBeReset()), this, SLOT(_k_modelAboutToBeReset()));
        disconnect(d->m_selectionModel.data()->model(), SIGNAL(modelReset()), this, SLOT(_k_modelReset()));
    }
    if (d->m_view && d->m_view.data()->model()) {
        connect(d->m_view.data()->model(), SIGNAL(modelAboutToBeReset()), SLOT(_k_modelAboutToBeReset()), Qt::UniqueConnection);
        connect(d->m_view.data()->model(), SIGNAL(modelReset()), SLOT(_k_modelReset()), Qt::UniqueConnection);
    }
}

#include "moc_kviewstatemaintainerbase.cpp"
