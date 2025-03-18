// SPDX-FileCopyrightText: 2025 g10 Code GmbH
// SPDX-FileContributor: Tobias Fella <tobias.fella@gnupg.com>
// SPDX-FileContributor: Carl Schwan <carl.schwan@gnupg.com>
// SPDX-License-Identifier: LGPL-2.0-or-later

#include "kplaceholderwidget.h"

#include <KTitleWidget>
#include <QAbstractItemView>
#include <QEvent>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class KPlaceholderWidgetPrivate
{
public:
    QLabel *icon;
    QVBoxLayout *layout;
    KTitleWidget *title;
    QLabel *label;
    QVBoxLayout *actionsLayout;
};

KPlaceholderWidget::KPlaceholderWidget(QWidget *parent)
    : QWidget(parent)
    , d(std::make_unique<KPlaceholderWidgetPrivate>())
{
    d->layout = new QVBoxLayout(this);
    d->layout->addStretch();
    d->icon = new QLabel(this);
    d->icon->setAlignment(Qt::AlignHCenter);
    d->layout->addWidget(d->icon);
    d->title = new KTitleWidget(this);

    d->layout->addWidget(d->title, 0, Qt::AlignHCenter);
    d->label = new QLabel();
    d->label->setAlignment(Qt::AlignHCenter);
    d->label->setWordWrap(true);
    d->layout->addWidget(d->label);

    d->actionsLayout = new QVBoxLayout();
    d->actionsLayout->addStretch();
    d->layout->addLayout(d->actionsLayout);

    if (auto view = qobject_cast<QAbstractItemView *>(parent); view) {
        view->installEventFilter(this);
    }
}

KPlaceholderWidget::~KPlaceholderWidget() = default;

bool KPlaceholderWidget::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj != parent()) {
        return QWidget::eventFilter(obj, ev);
    }

    if (ev->type() == QEvent::Resize) {
        if (auto view = qobject_cast<QAbstractItemView *>(obj); view) {
            setMaximumWidth(view->width() - /* gridUnit */ 18 * 4);
            move((view->width() - width()) / 2, (view->height() - height()) / 2);
        }
    }

    return false;
}

void KPlaceholderWidget::setTitle(const QString &title)
{
    d->title->setText(title);
}

QString KPlaceholderWidget::title() const
{
    return d->title->text();
}

void KPlaceholderWidget::setText(const QString &text)
{
    d->label->setText(text);
}

QString KPlaceholderWidget::text() const
{
    return d->label->text();
}

void KPlaceholderWidget::setIcon(const QIcon &icon)
{
    d->icon->setPixmap(icon.pixmap(128, 128));
}

void KPlaceholderWidget::setPixmap(const QPixmap &pixmap)
{
    d->icon->setPixmap(pixmap);
}

void KPlaceholderWidget::addAction(QAction *action)
{
    auto button = new QPushButton(action->icon(), action->text(), this);
    button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    d->actionsLayout->addWidget(button, 0, Qt::AlignHCenter);
    connect(button, &QPushButton::clicked, action, &QAction::triggered);
}
