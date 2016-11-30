/* This file is part of the KDE frameworks
 *
 * Copyright (c) 2011-2014 Aurélien Gâteau <agateau@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */
#include <QApplication>
#include <kcolumnresizer.h>

#include <kcolumnresizertestapp.h>

#include <ui_kcolumnresizertestapp.h>

Window::Window()
{
    m_ui = new Ui_ColumnResizerTestWidget;
    m_ui->setupUi(this);

    KColumnResizer *resizer = new KColumnResizer(this);
    resizer->addWidgetsFromLayout(m_ui->gridGroupBox1->layout(), 0);
    resizer->addWidgetsFromLayout(m_ui->formGroupBox1->layout(), 0);
    resizer->addWidgetsFromLayout(m_ui->formGroupBox2->layout(), 0);
    resizer->addWidgetsFromLayout(m_ui->gridGroupBox2->layout(), 0);

    connect(m_ui->spinBox, SIGNAL(valueChanged(int)), SLOT(updateAdjustableLabel()));
    updateAdjustableLabel();
}

Window::~Window()
{
    delete m_ui;
}

void Window::updateAdjustableLabel()
{
    QString txt;
    txt.fill(QLatin1Char('#'), m_ui->spinBox->value());
    m_ui->adjustableLabel->setText(QStringLiteral("Spin me %1:").arg(txt));
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    Window window;
    window.show();
    return app.exec();
}

