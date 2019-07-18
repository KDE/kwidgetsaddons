/*
 * Copyright (c) 2017 Elvis Angelaccio <elvis.angelaccio@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ktooltipwidget_test.h"

#include <KToolTipWidget>

#include <QApplication>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

// Doxygen will generate code snippets from this file.
// We can't use i18n() here, but we want it to show up in the apidox.
#define i18n QStringLiteral

TestDialog::TestDialog(QWidget *parent)
    : QDialog(parent)
{
    m_view.setColumnCount(1);
    m_view.setRootIsDecorated(false);
    m_view.setMouseTracking(true);

    auto item = new QTreeWidgetItem(&m_view);
    item->setText(0, QStringLiteral("Hover me! (first item)"));
    auto item2 = new QTreeWidgetItem(&m_view);
    item2->setText(0, QStringLiteral("Hover me! (second item)"));

    auto spinBox = new QSpinBox(this);
    spinBox->setPrefix(QStringLiteral("Hide delay (ms): "));
    spinBox->setMaximum(5000);
    spinBox->setValue(500);
    connect(spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), &m_tooltipWidget, &KToolTipWidget::setHideDelay);
    auto layout = new QVBoxLayout(this);
    layout->addWidget(spinBox);
    layout->addWidget(&m_view);

    //! [show_tooltip_widget]
    connect(&m_view, &QAbstractItemView::entered, this, [=](const QModelIndex &index) {
        auto rect = m_view.visualRect(index);
        const auto pos = m_view.viewport()->mapToGlobal(rect.topLeft());
        rect.moveTo(pos);
        auto button = new QPushButton(i18n("Push me (row %1)").arg(index.row()), this);
        m_tooltipWidget.showBelow(rect, button, m_view.nativeParentWidget()->windowHandle());
    });
    //! [show_tooltip_widget]

    connect(&m_view, &QAbstractItemView::viewportEntered, &m_tooltipWidget, &KToolTipWidget::hideLater);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    TestDialog dialog;
    QObject::connect(&dialog, &QDialog::finished, &app, &QCoreApplication::quit);
    dialog.show();

    return app.exec();
}

