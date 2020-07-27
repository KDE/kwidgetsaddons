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
#include <QStyle>
#include <QEvent>


void KPageViewPrivate::_k_rebuildGui()
{
    // clean up old view
    Q_Q(KPageView);

    QModelIndex currentLastIndex;
    if (view && view->selectionModel()) {
        QObject::disconnect(view->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                            q, SLOT(_k_pageSelected(QItemSelection,QItemSelection)));
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
        QObject::connect(view->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), q, SLOT(_k_pageSelected(QItemSelection,QItemSelection)));

        if (currentLastIndex.isValid()) {
            view->selectionModel()->setCurrentIndex(currentLastIndex, QItemSelectionModel::Select);
        } else if (model) {
            view->selectionModel()->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);
        }
    }

    // update header visibility
    const bool isPageHeaderShown = q->showPageHeader();
    if (pageHeader) {
        pageHeader->setVisible(isPageHeaderShown);
    } else {
        titleWidget->setVisible(isPageHeaderShown);
    }

    // update main page with either stack or tabbed view
    pageLayout->removeWidget(stack);

    if (faceType == KPageView::Tabbed) {
        // view should use most space, so stretch with 1
        pageLayout->insertWidget(1, view, 1);
        stack->setVisible(false);
    } else {
        // stack should use most space, so stretch with 1
        pageLayout->insertWidget(1, stack, 1);
        stack->setVisible(true);
    }

    // main layout
    if (faceType != KPageView::Tabbed && effectiveFaceType() != KPageView::Plain) {
        Qt::Alignment alignment = q->viewPosition();
        if (alignment & Qt::AlignLeft) {
            mainLayout->insertWidget(0, view);
        } else if (alignment & Qt::AlignRight) {
            mainLayout->addWidget(view);
        } else {
            qCWarning(KWidgetsAddonsLog) << "Unsupported navigation view position:" << alignment;
        }
    }
    updatePageMargins();
}

void KPageViewPrivate::updatePageMargins()
{
    Q_Q(KPageView);

    auto *style = q->style();

    int margin;
    if (pageMarginsMode == KPageView::NoPageMargins) {
        margin = 0;
    } else {
        // TODO: PM_DefaultTopLevelMargin is deprecated for Qt 6, but for now there is no
        // other way to have the same toplevel margin used also for other dialogs
        margin = style->pixelMetric(QStyle::PM_DefaultTopLevelMargin);
    }
    int leftMargin = margin;
    const int topMargin = margin;
    int rightMargin = margin;
    const int bottomMargin = margin;

    if (faceType != KPageView::Tabbed && effectiveFaceType() != KPageView::Plain) {
        const Qt::Alignment alignment = q->viewPosition();
        if (alignment & Qt::AlignLeft) {
            leftMargin = style->pixelMetric(QStyle::PM_LayoutLeftMargin);
        } else if (alignment & Qt::AlignRight) {
            rightMargin = style->pixelMetric(QStyle::PM_LayoutRightMargin);
        }
    }

    pageLayout->setContentsMargins(leftMargin, topMargin, rightMargin, bottomMargin);
    pageLayout->setSpacing(style->pixelMetric(QStyle::PM_LayoutVerticalSpacing));
}

void KPageViewPrivate::updateSelection()
{
    /**
     * Select the first item in the view if not done yet.
     */

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
    /**
     * Remove all orphan pages from the stacked widget.
     */

    const QList<QWidget *> widgets = collectPages();

    for (int i = 0; i < stack->count(); ++i) {
        QWidget *page = stack->widget(i);

        bool found = false;
        for (int j = 0; j < widgets.count(); ++j) {
            if (widgets[ j ] == page) {
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
    /**
     * Traverse through the model recursive and collect all widgets in
     * a list.
     */
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

    /**
     * Check whether the model has sub pages.
     */
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

void KPageViewPrivate::_k_modelChanged()
{
    if (!model) {
        return;
    }

    /**
     * If the face type is Auto, we rebuild the GUI whenever the layout
     * of the model changes.
     */
    if (faceType == KPageView::Auto) {
        _k_rebuildGui();
        // If you discover some crashes use the line below instead...
        //QTimer::singleShot(0, q, SLOT(_k_rebuildGui()));
    }

    /**
     * Set the stack to the minimum size of the largest widget.
     */
    QSize size = stack->size();
    const QList<QWidget *> widgets = collectPages();
    for (int i = 0; i < widgets.count(); ++i) {
        const QWidget *widget = widgets[ i ];
        if (widget) {
            size = size.expandedTo(widget->minimumSizeHint());
        }
    }
    stack->setMinimumSize(size);

    updateSelection();
}

void KPageViewPrivate::_k_pageSelected(const QItemSelection &index, const QItemSelection &previous)
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
            if (stack->indexOf(widget) == -1) {     // not included yet
                stack->addWidget(widget);
            }

            stack->setCurrentWidget(widget);
        } else {
            stack->setCurrentWidget(defaultWidget);
        }

        updateTitleWidget(currentIndex);
    }

    Q_Q(KPageView);
    emit q->currentPageChanged(currentIndex, previousIndex);
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
    if (header.isNull()) {   //TODO KDE5 remove that ugly logic, see also doxy-comments in KPageWidgetItem::setHeader()
        header = model->data(index, Qt::DisplayRole).toString();
    }

    titleWidget->setText(header);

    titleWidget->setVisible(q->showPageHeader());
}

void KPageViewPrivate::_k_dataChanged(const QModelIndex &, const QModelIndex &)
{
    /**
     * When data has changed we update the header and icon for the currently selected
     * page.
     */
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
    , pageMarginsMode(KPageView::NoPageMargins)
    , mainLayout(nullptr)
    , pageLayout(nullptr)
    , stack(nullptr)
    , titleWidget(nullptr)
    , view(nullptr)
{
}

void KPageViewPrivate::init()
{
    Q_Q(KPageView);
    mainLayout = new QHBoxLayout(q);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    pageLayout = new QVBoxLayout;

    stack = new KPageStackedWidget(q);
    titleWidget = new KTitleWidget(q);

    defaultWidget = new QWidget(q);
    stack->addWidget(defaultWidget);

    // initial layout
    pageLayout->addWidget(titleWidget);
    // stack should use most space, so stretch with 1
    pageLayout->addWidget(stack, 1);

    // page should use most space, so stretch with 1
    mainLayout->addLayout(pageLayout, 1);

    updatePageMargins();
}

/**
 * KPageView Implementation
 */
KPageView::KPageView(QWidget *parent)
    : KPageView(*new KPageViewPrivate(this), parent)
{
}

KPageView::KPageView(KPageViewPrivate &dd, QWidget *parent)
    : QWidget(parent), d_ptr(&dd)
{
    d_ptr->init();
}

KPageView::~KPageView()
{
    delete d_ptr;
}

void KPageView::setModel(QAbstractItemModel *model)
{
    Q_D(KPageView);
    // clean up old model
    if (d->model) {
        disconnect(d->model, SIGNAL(layoutChanged()), this, SLOT(_k_modelChanged()));
        disconnect(d->model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                   this, SLOT(_k_dataChanged(QModelIndex,QModelIndex)));
    }

    d->model = model;

    if (d->model) {
        connect(d->model, SIGNAL(layoutChanged()), this, SLOT(_k_modelChanged()));
        connect(d->model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                this, SLOT(_k_dataChanged(QModelIndex,QModelIndex)));

        // set new model in navigation view
        if (d->view) {
            d->view->setModel(model);
        }
    }

    d->_k_rebuildGui();
}

QAbstractItemModel *KPageView::model() const
{
    Q_D(const KPageView);
    return d->model;
}

void KPageView::setFaceType(FaceType faceType)
{
    Q_D(KPageView);

    if (d->faceType == faceType) {
        return;
    }

    d->faceType = faceType;

    d->_k_rebuildGui();
}

KPageView::FaceType KPageView::faceType() const
{
    Q_D(const KPageView);
    return d->faceType;
}

void KPageView::setPageMarginsMode(KPageView::PageMarginsMode pageMarginsMode)
{
    Q_D(KPageView);

    if (d->pageMarginsMode == pageMarginsMode) {
        return;
    }

    d->pageMarginsMode = pageMarginsMode;

    d->updatePageMargins();
}


KPageView::PageMarginsMode KPageView::pageMarginsMode() const
{
    Q_D(const KPageView);
    return d->pageMarginsMode;
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
        d->pageLayout->removeWidget(d->pageHeader);
    } else {
        d->pageLayout->removeWidget(d->titleWidget);
    }

    d->pageHeader = header;

    // insert always as first, but set invisible in case
    if (d->pageHeader) {
        d->pageLayout->insertWidget(0, d->pageHeader);
        d->pageHeader->setVisible(showPageHeader());
    } else {
        d->pageLayout->insertWidget(0, d->titleWidget);
        d->titleWidget->setVisible(showPageHeader());
    }
}

QWidget *KPageView::pageHeader() const
{
    Q_D(const KPageView);
    return d->pageHeader;
}

void KPageView::setPageFooter(QWidget *footer)
{
    Q_D(KPageView);
    if (d->pageFooter == footer) {
        return;
    }

    if (d->pageFooter) {
        d->pageLayout->removeWidget(d->pageFooter);
    }

    d->pageFooter = footer;

    if (footer) {
        // appending as last
        d->pageLayout->addWidget(d->pageFooter);
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
    if (faceType == List) {
        return new KDEPrivate::KPageListView(this);
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

void KPageView::changeEvent(QEvent* event)
{
    Q_D(KPageView);

    QWidget::changeEvent(event);
    if (event->type() == QEvent::StyleChange) {
        d->updatePageMargins();
    }
}

#include "moc_kpageview.cpp"
