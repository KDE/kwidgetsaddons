/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>
    SPDX-FileContributor: Stephen Kelly <stephen@kdab.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KVIEWSTATESERIALIZER_H
#define KVIEWSTATESERIALIZER_H

#include <QObject>
#include <QPair>
#include <QStringList>
#include <memory>

#include "kwidgetsaddons_export.h"

class QAbstractItemView;
class QItemSelectionModel;
class QAbstractItemModel;
class QAbstractScrollArea;
class QModelIndex;

class KViewStateSerializerPrivate;

/*!
 * \class KViewStateSerializer
 * \inmodule KWidgetsAddons
 *
 * \brief Object for saving and restoring state in QTreeViews and QItemSelectionModels.
 *
 * Implement the indexFromConfigString and indexToConfigString methods to handle
 * the model in the view whose state is being saved. These implementations can be
 * quite trivial:
 *
 * \code
 *   QModelIndex DynamicTreeStateSaver::indexFromConfigString(const QAbstractItemModel* model,
 *                                                            const QString& key) const
 *   {
 *       QModelIndexList list = model->match(model->index(0, 0),
 *                                           DynamicTreeModel::DynamicTreeModelId,
 *                                           key.toInt(), 1, Qt::MatchRecursive);
 *       if (list.isEmpty()) {
 *           return QModelIndex();
 *       }
 *       return list.first();
 *   }
 *
 *   QString DynamicTreeStateSaver::indexToConfigString(const QModelIndex& index) const
 *   {
 *       return index.data(DynamicTreeModel::DynamicTreeModelId).toString();
 *   }
 * \endcode
 *
 * It is possible to restore the state of a QTreeView (that is, the expanded state and
 * selected state of all indexes as well as the horizontal and vertical scroll state) by
 * using setView.
 *
 * If there is no tree view state to restore (for example if using QML), the selection
 * state of a QItemSelectionModel can be saved or restored instead.
 *
 * The state of any QAbstractScrollArea can also be saved and restored.
 *
 * A KViewStateSerializer should be created on the stack when saving and on the heap
 * when restoring. The model may be populated dynamically between several event loops,
 * so it may not be immediate for the indexes that should be selected to be in the model.
 * The saver should *not* be persisted as a member. The saver will destroy itself when it
 * has completed the restoration specified in the config group, or a small amount of time
 * has elapsed.
 *
 * \code
 *   MyWidget::MyWidget(Qobject *parent)
 *     : QWidget(parent)
 *   {
 *       ...
 *
 *       m_view = new QTreeView(splitter);
 *       m_view->setModel(model);
 *
 *       connect(model, &QAbstractItemModel::modelAboutToBeReset, this, [this]() { saveState(); });
 *       connect(model, &QAbstractItemModel::modelReset, [this]() { restoreState(); });
 *       connect(qApp, &QApplication::aboutToQuit, this, [this]() { saveState(); });
 *
 *       restoreState();
 *   }
 *
 *   void StateSaverWidget::saveState()
 *   {
 *       ConcreteStateSaver saver;
 *       saver.setView(m_view);
 *
 *       KConfigGroup cfg(KSharedConfig::openConfig(), "ExampleViewState");
 *       saver.saveState(cfg);
 *       cfg.sync();
 *   }
 *
 *   void StateSaverWidget::restoreState()
 *   {
 *       // Will delete itself.
 *       ConcreteTreeStateSaver *saver = new ConcreteStateSaver();
 *       saver->setView(m_view);
 *       KConfigGroup cfg(KSharedConfig::openConfig(), "ExampleViewState");
 *       saver->restoreState(cfg);
 *   }
 * \endcode
 *
 * After creating a saver, the state can be saved using a KConfigGroup.
 *
 * It is also possible to save and restore state directly by using the restoreSelection,
 * restoreExpanded etc methods. Note that the implementation of these methods should return
 * strings that the indexFromConfigString implementation can handle.
 *
 * \code
 *   class DynamicTreeStateSaver : public KViewStateSerializer
 *   {
 *       Q_OBJECT
 *   public:
 *     // ...
 *
 *       void selectItems(const QList<qint64> &items)
 *       {
 *           QStringList itemStrings;
 *           for (qint64 item : items) {
 *               itemStrings << QString::number(item);
 *           }
 *           restoreSelection(itemStrings);
 *       }
 *
 *       void expandItems(const QList<qint64> &items)
 *       {
 *           QStringList itemStrings;
 *           for (qint64 item : items) {
 *               itemStrings << QString::number(item);
 *           }
 *           restoreSelection(itemStrings);
 *       }
 *   };
 * \endcode
 *
 * Note that a single instance of this class should be used with only one widget.
 * That is don't do this:
 *
 * \code
 *   saver->setView(treeView1);
 *   saver->setSelectionModel(treeView2->selectionModel());
 *   saver->setScrollArea(treeView3);
 * \endcode
 *
 * To save the state of 3 different widgets, use three savers, even if they operate
 * on the same root model.
 *
 * \code
 *   saver1->setView(treeView1);
 *   saver2->setSelectionModel(treeView2->selectionModel());
 *   saver3->setScrollArea(treeView3);
 * \endcode
 *
 * \note The KViewStateSerializer does not take ownership of any widgets set on it.
 *
 * It is recommended to restore the state on application startup and after the
 * model has been reset, and to save the state on application close and before
 * the model has been reset.
 *
 * \sa QAbstractItemModel::modelAboutToBeReset(), QAbstractItemModel::modelReset()
 *
 * \since 4.5
 */
class KWIDGETSADDONS_EXPORT KViewStateSerializer : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructor
     */
    explicit KViewStateSerializer(QObject *parent = nullptr);

    ~KViewStateSerializer() override;

    /*!
     * The view whose state is persisted.
     */
    QAbstractItemView *view() const;

    /*!
     * Sets the view whose state is persisted.
     */
    void setView(QAbstractItemView *view);

    /*!
     * The QItemSelectionModel whose state is persisted.
     */
    QItemSelectionModel *selectionModel() const;

    /*!
     * Sets the QItemSelectionModel whose state is persisted.
     */
    void setSelectionModel(QItemSelectionModel *selectionModel);

    /*!
     * Returns a QStringList describing the selection in the selectionModel.
     */
    QStringList selectionKeys() const;

    /*!
     * Returns a QStringList representing the expanded indexes in the QTreeView.
     */
    QStringList expansionKeys() const;

    /*!
     * Returns a QString describing the current index in the selection model.
     */
    QString currentIndexKey() const;

    /*!
     * Returns the vertical and horizontal scroll of the QAbstractScrollArea.
     */
    QPair<int, int> scrollState() const;

    /*!
     * Select the indexes described by \a indexStrings
     */
    void restoreSelection(const QStringList &indexStrings);

    /*!
     * Make the index described by \a indexString the currentIndex in the selectionModel.
     */
    void restoreCurrentItem(const QString &indexString);

    /*!
     * Expand the indexes described by \a indexStrings in the QTreeView.
     */
    void restoreExpanded(const QStringList &indexStrings);

    /*!
     * Restores the scroll state of the QAbstractScrollArea to the \a verticalScoll
     * and \a horizontalScroll
     */
    void restoreScrollState(int verticalScoll, int horizontalScroll);

protected:
    /*!
     * Reimplement to return an index in the \a model described by the unique key \a key
     */
    virtual QModelIndex indexFromConfigString(const QAbstractItemModel *model, const QString &key) const = 0;

    /*!
     * Reimplement to return a unique string for the \a index.
     */
    virtual QString indexToConfigString(const QModelIndex &index) const = 0;

    void restoreState();

private:
    Q_DECLARE_PRIVATE(KViewStateSerializer)
    std::unique_ptr<KViewStateSerializerPrivate> const d_ptr;
};

#endif
