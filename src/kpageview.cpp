/*
    This file is part of the KDE Libraries
    SPDX-FileCopyrightText: 2006 Tobias Koenig <tokoe@kde.org>
    SPDX-FileCopyrightText: 2007 Rafael Fernández López <ereslibre@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kpageview.h"
#include "kpageview_p.h"

#include "kpagemodel.h"
#include "loggingcategory.h"

#include <ktitlewidget.h>

#include <QAbstractItemView>
#include <QGridLayout>
#include <QSize>
#include <QTimer>

void KPageViewPrivate::rebuildGui()
{
    // clean up old view
    Q_Q(KPageView);

    QModelIndex currentLastIndex;
    if (view && view->selectionModel()) {
        QObject::disconnect(m_selectionChangedConnection);
        currentLastIndex = view->selectionModel()->currentIndex();
    }

    delete view;
    view = q->createView();

    Q_ASSERT(view);

    view->setSelectionBehavior(QAbstractItemView::SelectItems);
    view->setSelectionMode(QAbstractItemView::SingleSelection);

    if (model) {
        view->setModel(model);
    }

    // setup new view
    if (view->selectionModel()) {
        m_selectionChangedConnection = QObject::connect(view->selectionModel(),
                                                        &QItemSelectionModel::selectionChanged,
                                                        q,
                                                        [this](const QItemSelection &selected, const QItemSelection &deselected) {
                                                            pageSelected(selected, deselected);
                                                        });

        if (currentLastIndex.isValid()) {
            view->selectionModel()->setCurrentIndex(currentLastIndex, QItemSelectionModel::Select);
        } else if (model) {
            view->selectionModel()->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);
        }
    }

    if (faceType == KPageView::Tabbed) {
        stack->setVisible(false);
        layout->removeWidget(stack);
    } else {
        layout->addWidget(stack, 2, 1);
        stack->setVisible(true);
    }

    layout->removeWidget(titleWidget);

    if (pageHeader) {
        layout->removeWidget(pageHeader);
        pageHeader->setVisible(q->showPageHeader());
        titleWidget->setVisible(false);

        if (faceType == KPageView::Tabbed) {
            layout->addWidget(pageHeader, 1, 1);
        } else {
            layout->addWidget(pageHeader, 1, 1, 1, 2);
        }
    } else {
        titleWidget->setVisible(q->showPageHeader());
        if (faceType == KPageView::Tabbed) {
            layout->addWidget(titleWidget, 1, 1);
        } else {
            layout->addWidget(titleWidget, 1, 1, 1, 2);
        }
    }

    Qt::Alignment alignment = q->viewPosition();
    if (alignment & Qt::AlignTop) {
        layout->addWidget(view, 2, 1);
    } else if (alignment & Qt::AlignRight) {
        layout->addWidget(view, 1, 2, 4, 1);
    } else if (alignment & Qt::AlignBottom) {
        layout->addWidget(view, 4, 1);
    } else if (alignment & Qt::AlignLeft) {
        layout->addWidget(view, 1, 0, 4, 1);
    }
}

void KPageViewPrivate::updateSelection()
{
    // Select the first item in the view if not done yet.

    if (!model) {
        return;
    }

    if (!view || !view->selectionModel()) {
        return;
    }

    const QModelIndex index = view->selectionModel()->currentIndex();
    if (!index.isValid()) {
        view->selectionModel()->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);
    }
}

void KPageViewPrivate::cleanupPages()
{
    // Remove all orphan pages from the stacked widget.

    const QList<QWidget *> widgets = collectPages();

    for (int i = 0; i < stack->count(); ++i) {
        QWidget *page = stack->widget(i);

        bool found = false;
        for (int j = 0; j < widgets.count(); ++j) {
            if (widgets[j] == page) {
                found = true;
            }
        }

        if (!found) {
            stack->removeWidget(page);
        }
    }
}

QList<QWidget *> KPageViewPrivate::collectPages(const QModelIndex &parentIndex)
{
    // Traverse through the model recursive and collect all widgets in
    // a list.
    QList<QWidget *> retval;

    int rows = model->rowCount(parentIndex);
    for (int j = 0; j < rows; ++j) {
        const QModelIndex index = model->index(j, 0, parentIndex);
        retval.append(qvariant_cast<QWidget *>(model->data(index, KPageModel::WidgetRole)));

        if (model->rowCount(index) > 0) {
            retval += collectPages(index);
        }
    }

    return retval;
}

KPageView::FaceType KPageViewPrivate::effectiveFaceType() const
{
    if (faceType == KPageView::Auto) {
        return detectAutoFace();
    }

    return faceType;
}

KPageView::FaceType KPageViewPrivate::detectAutoFace() const
{
    if (!model) {
        return KPageView::Plain;
    }

    // Check whether the model has sub pages.
    bool hasSubPages = false;
    const int count = model->rowCount();
    for (int i = 0; i < count; ++i) {
        if (model->rowCount(model->index(i, 0)) > 0) {
            hasSubPages = true;
            break;
        }
    }

    if (hasSubPages) {
        return KPageView::Tree;
    }

    if (model->rowCount() > 1) {
        return KPageView::List;
    }

    return KPageView::Plain;
}

void KPageViewPrivate::modelChanged()
{
    if (!model) {
        return;
    }

    // If the face type is Auto, we rebuild the GUI whenever the layout
    // of the model changes.
    if (faceType == KPageView::Auto) {
        rebuildGui();
        // If you discover some crashes use the line below instead...
        // QTimer::singleShot(0, q, SLOT(rebuildGui()));
    }

    // Set the stack to the minimum size of the largest widget.
    QSize size = stack->size();
    const QList<QWidget *> widgets = collectPages();
    for (int i = 0; i < widgets.count(); ++i) {
        const QWidget *widget = widgets[i];
        if (widget) {
            size = size.expandedTo(widget->minimumSizeHint());
        }
    }
    stack->setMinimumSize(size);

    updateSelection();
}

void KPageViewPrivate::pageSelected(const QItemSelection &index, const QItemSelection &previous)
{
    if (!model) {
        return;
    }

    // Return if the current Index is not valid
    if (index.indexes().size() != 1) {
        return;
    }
    QModelIndex currentIndex = index.indexes().first();

    QModelIndex previousIndex;
    // The previous index can be invalid
    if (previous.indexes().size() == 1) {
        previousIndex = previous.indexes().first();
    }

    if (faceType != KPageView::Tabbed) {
        QWidget *widget = qvariant_cast<QWidget *>(model->data(currentIndex, KPageModel::WidgetRole));

        if (widget) {
            if (stack->indexOf(widget) == -1) { // not included yet
                stack->addWidget(widget);
            }

            stack->setCurrentWidget(widget);
        } else {
            stack->setCurrentWidget(defaultWidget);
        }

        updateTitleWidget(currentIndex);
    }

    Q_Q(KPageView);
    Q_EMIT q->currentPageChanged(currentIndex, previousIndex);
}

void KPageViewPrivate::updateTitleWidget(const QModelIndex &index)
{
    Q_Q(KPageView);

    const bool headerVisible = model->data(index, KPageModel::HeaderVisibleRole).toBool();
    if (!headerVisible) {
        titleWidget->setVisible(false);
        return;
    }
    QString header = model->data(index, KPageModel::HeaderRole).toString();
    if (header.isNull()) { // TODO KF6 remove that ugly logic, see also doxy-comments in KPageWidgetItem::setHeader()
        header = model->data(index, Qt::DisplayRole).toString();
    }

    titleWidget->setText(header);

    titleWidget->setVisible(q->showPageHeader());
}

void KPageViewPrivate::dataChanged(const QModelIndex &, const QModelIndex &)
{
    // When data has changed we update the header and icon for the currently selected
    // page.
    if (!view) {
        return;
    }

    QModelIndex index = view->selectionModel()->currentIndex();
    if (!index.isValid()) {
        return;
    }

    updateTitleWidget(index);
}

KPageViewPrivate::KPageViewPrivate(KPageView *_parent)
    : q_ptr(_parent)
    , model(nullptr)
    , faceType(KPageView::Auto)
    , layout(nullptr)
    , stack(nullptr)
    , titleWidget(nullptr)
    , view(nullptr)
{
}

void KPageViewPrivate::init()
{
    Q_Q(KPageView);
    layout = new QGridLayout(q);
    stack = new KPageStackedWidget(q);
    titleWidget = new KTitleWidget(q);
    layout->addWidget(titleWidget, 1, 1, 1, 2);
    layout->addWidget(stack, 2, 1);

    defaultWidget = new QWidget(q);
    stack->addWidget(defaultWidget);

    // stack should use most space
    layout->setColumnStretch(1, 1);
    layout->setRowStretch(2, 1);
}

// KPageView Implementation
KPageView::KPageView(QWidget *parent)
    : KPageView(*new KPageViewPrivate(this), parent)
{
}

KPageView::KPageView(KPageViewPrivate &dd, QWidget *parent)
    : QWidget(parent)
    , d_ptr(&dd)
{
    d_ptr->init();
}

KPageView::~KPageView() = default;

void KPageView::setModel(QAbstractItemModel *model)
{
    Q_D(KPageView);
    // clean up old model
    if (d->model) {
        disconnect(d->m_layoutChangedConnection);
        disconnect(d->m_dataChangedConnection);
    }

    d->model = model;

    if (d->model) {
        d->m_layoutChangedConnection = connect(d->model, &QAbstractItemModel::layoutChanged, this, [d]() {
            d->modelChanged();
        });
        d->m_dataChangedConnection = connect(d->model, &QAbstractItemModel::dataChanged, this, [d](const QModelIndex &topLeft, const QModelIndex &bottomRight) {
            d->dataChanged(topLeft, bottomRight);
        });

        // set new model in navigation view
        if (d->view) {
            d->view->setModel(model);
        }
    }

    d->rebuildGui();
}

QAbstractItemModel *KPageView::model() const
{
    Q_D(const KPageView);
    return d->model;
}

void KPageView::setFaceType(FaceType faceType)
{
    Q_D(KPageView);
    d->faceType = faceType;

    d->rebuildGui();
}

KPageView::FaceType KPageView::faceType() const
{
    Q_D(const KPageView);
    return d->faceType;
}

void KPageView::setCurrentPage(const QModelIndex &index)
{
    Q_D(KPageView);
    if (!d->view || !d->view->selectionModel()) {
        return;
    }

    d->view->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
}

QModelIndex KPageView::currentPage() const
{
    Q_D(const KPageView);
    if (!d->view || !d->view->selectionModel()) {
        return QModelIndex();
    }

    return d->view->selectionModel()->currentIndex();
}

void KPageView::setItemDelegate(QAbstractItemDelegate *delegate)
{
    Q_D(KPageView);
    if (d->view) {
        d->view->setItemDelegate(delegate);
    }
}

QAbstractItemDelegate *KPageView::itemDelegate() const
{
    Q_D(const KPageView);
    if (d->view) {
        return d->view->itemDelegate();
    } else {
        return nullptr;
    }
}

void KPageView::setDefaultWidget(QWidget *widget)
{
    Q_D(KPageView);

    Q_ASSERT(widget);

    bool isCurrent = (d->stack->currentIndex() == d->stack->indexOf(d->defaultWidget));

    // remove old default widget
    d->stack->removeWidget(d->defaultWidget);
    delete d->defaultWidget;

    // add new default widget
    d->defaultWidget = widget;
    d->stack->addWidget(d->defaultWidget);

    if (isCurrent) {
        d->stack->setCurrentWidget(d->defaultWidget);
    }
}

void KPageView::setPageHeader(QWidget *header)
{
    Q_D(KPageView);
    if (d->pageHeader == header) {
        return;
    }

    if (d->pageHeader) {
        d->layout->removeWidget(d->pageHeader);
    }
    d->layout->removeWidget(d->titleWidget);

    d->pageHeader = header;

    // Give it a colSpan of 2 to add a margin to the right
    if (d->pageHeader) {
        d->layout->addWidget(d->pageHeader, 1, 1, 1, 2);
        d->pageHeader->setVisible(showPageHeader());
    } else {
        d->layout->addWidget(d->titleWidget, 1, 1, 1, 2);
        d->titleWidget->setVisible(showPageHeader());
    }
}

QWidget *KPageView::pageHeader() const
{
    Q_D(const KPageView);
    if (!d->pageHeader) {
        return d->titleWidget;
    }
    return d->pageHeader;
}

void KPageView::setPageFooter(QWidget *footer)
{
    Q_D(KPageView);
    if (d->pageFooter == footer) {
        return;
    }

    if (d->pageFooter) {
        d->layout->removeWidget(d->pageFooter);
    }

    d->pageFooter = footer;

    if (footer) {
        d->layout->addWidget(d->pageFooter, 3, 1);
    }
}

QWidget *KPageView::pageFooter() const
{
    Q_D(const KPageView);
    return d->pageFooter;
}

QAbstractItemView *KPageView::createView()
{
    Q_D(KPageView);
    const FaceType faceType = d->effectiveFaceType();

    if (faceType == Plain) {
        return new KDEPrivate::KPagePlainView(this);
    }
    if (faceType == FlatList) {
        return new KDEPrivate::KPageListView(this);
    }
    if (faceType == List) {
        auto view = new KDEPrivate::KPageListView(this);
        view->setItemDelegate(new KDEPrivate::KPageListViewDelegate(this));
        return view;
    }
    if (faceType == Tree) {
        return new KDEPrivate::KPageTreeView(this);
    }
    if (faceType == Tabbed) {
        return new KDEPrivate::KPageTabbedView(this);
    }

    return nullptr;
}

bool KPageView::showPageHeader() const
{
    Q_D(const KPageView);
    const FaceType faceType = d->effectiveFaceType();

    if (faceType == Tabbed) {
        return false;
    } else {
        return d->pageHeader || !d->titleWidget->text().isEmpty();
    }
}

Qt::Alignment KPageView::viewPosition() const
{
    Q_D(const KPageView);
    const FaceType faceType = d->effectiveFaceType();

    if (faceType == Plain || faceType == Tabbed) {
        return Qt::AlignTop;
    } else {
        return Qt::AlignLeft;
    }
}

#include "moc_kpageview.cpp"
