// SPDX-FileCopyrightText: 2025 g10 Code GmbH
// SPDX-FileContributor: Tobias Fella <tobias.fella@gnupg.com>
// SPDX-FileContributor: Carl Schwan <carl.schwan@gnupg.com>
// SPDX-License-Identifier: LGPL-2.0-or-later

#pragma once

#include <QWidget>
#include <kwidgetsaddons_export.h>

/**
 * @class KPlaceholderWidget kplaceholderwiget.h KPlaceholderWidget
 *
 * @short A standard placeholder which can be used when an object is empty.
 *
 * Usage with a QStackedLayout to display a placeholder when something isn't available:
 *
 * @code
 * auto stackedLayout = new QStackedLayout(layout);
 *
 * auto viewerWidget = new MyViewerWidget();
 *
 * auto placeholderWidget = new KPlaceholderWidget(this);
 * auto retryAction = new QAction(QIcon::fromTheme(u"document-decrypt"_s), i18nc("@action:button", "Retry decryption"));
 * placeholderWidget->addAction(retryAction);
 *
 * stackedLayout->addWidget(viewerWidget);
 * stackedLayout->addWidget(placeholderWidget);
 *
 * // Decide which layer to display
 * if (loadingJob->error() == KJob::NoError) {
 *     stackedLayout->setCurrentIndex(0);
 * } else {
 *     placeholderWidget->setIcon(QIcon::fromTheme(u"data-error"_s));
 *     placeholderWidget->setTitle(i18nc("@title", "Failed to Decrypt Password"));
 *     placeholderWidget->setText(decryptJob->errorString());
 *     stackedLayout->setCurrentIndex(1);
 * }
 * @endcode
 *
 * Usage in a QTreeView when empty
 *
 * @code
 * auto treeView = new QTreeView(this);
 * treeView->setModel(myModel);
 *
 * auto placeholderWidget = new KPlaceholderWidget(treeView);
 * placeholderWidget->setTitle(i18nc("@title", "No entry found"));
 *
 * auto updatePlaceholder = [myModel](const QModelIndex &index = {}, int start = 0, int end = 0) {
 *     placeholderWidget->setVisible(!myModel->hasChildren(myModel.index(0, 0)));
 * };
 *
 * connect(myModel, &QAbstractListModel::rowsInserted, this, updatePlaceholder);
 * connect(myModel, &QAbstractListModel::rowsRemoved, this, updatePlaceholder);
 * connect(treeView, &QTreeView::resized, this, updatePlaceholderPosition);
 *
 * updatePlaceholder();
 * @endcode
 *
 * @since 6.13
 * @author Tobias Fella <tobias.fella@gnupg.com>
 * @author Carl Schwan <carl.schwan@gnupg.com>
 */
class KWIDGETSADDONS_EXPORT KPlaceholderWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * Contructor
     * @param parent The parent widget.
     */
    explicit KPlaceholderWidget(QWidget *parent = nullptr);
    ~KPlaceholderWidget();

    /**
     * Get the title associated with the placeholder widget.
     */
    [[nodiscard]] QString title() const;

    /**
     * Set the title associated with the placeholder widget.
     * @param title The new title
     */
    void setTitle(const QString &title);

    /**
     * Get the text with explanation associated with the placeholder widget.
     */
    [[nodiscard]] QString text() const;

    /**
     * Set the text with explanation associated with the placeholder widget.
     * @param text The new text
     */
    void setText(const QString &text);

    /**
     * Set the icon associated with the placeholder widget.
     * @param icon The new icon
     * @see setPixmap
     */
    void setIcon(const QIcon &icon);

    /**
     * Set the pixmap associated with the placeholder widget.
     * @param pixmap The new icon as a pixmap
     * @see setIcon
     */
    void setPixmap(const QPixmap &pixmap);

    /**
     * Add a action button to the placeholder widget.
     * @param action The action to add. The placeholder widget does not take ownership over the lifetime of the action.
     */
    void addAction(QAction *action);

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    std::unique_ptr<class KPlaceholderWidgetPrivate> d;
};
