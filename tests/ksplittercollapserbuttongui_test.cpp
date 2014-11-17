/*
  Copyright (c) 2014 Montel Laurent <montel@kde.org>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) version 3, or any
  later version accepted by the membership of KDE e.V. (or its
  successor approved by the membership of KDE e.V.), which shall
  act as a proxy defined in Section 6 of version 3 of the license.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "ksplittercollapserbuttongui_test.h"
#include "ksplittercollapserbutton.h"
#include <QSplitter>
#include <qapplication.h>

#include <QTextEdit>
#include <QHBoxLayout>
#include <QApplication>
#include <QCommandLineParser>

KSplitterCollapserButtonGui_test::KSplitterCollapserButtonGui_test(int indexOfWidgetAssociateToSplitterCollapser, Qt::Orientation orientation, QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *lay = new QHBoxLayout;
    setLayout(lay);
    QSplitter *splitter = new QSplitter(orientation);
    lay->addWidget(splitter);
    QTextEdit *firstTextEdit = new QTextEdit;
    splitter->addWidget(firstTextEdit);
    QTextEdit *secondTextEdit = new QTextEdit;
    splitter->addWidget(secondTextEdit);
    if (indexOfWidgetAssociateToSplitterCollapser == 0) {
        new KSplitterCollapserButton(firstTextEdit, splitter);
    } else {
        new KSplitterCollapserButton(secondTextEdit, splitter);
    }
}

KSplitterCollapserButtonGui_test::~KSplitterCollapserButtonGui_test()
{

}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);

    KSplitterCollapserButtonGui_test *w = new KSplitterCollapserButtonGui_test(0, Qt::Horizontal);
    w->resize(800, 600);
    w->show();

    KSplitterCollapserButtonGui_test *w2 = new KSplitterCollapserButtonGui_test(1, Qt::Horizontal);
    w2->resize(800, 600);
    w2->show();

    KSplitterCollapserButtonGui_test *w3 = new KSplitterCollapserButtonGui_test(0, Qt::Vertical);
    w3->resize(800, 600);
    w3->show();

    KSplitterCollapserButtonGui_test *w4 = new KSplitterCollapserButtonGui_test(1, Qt::Vertical);
    w4->resize(800, 600);
    w4->show();

    app.exec();
    delete w;
    delete w2;
    delete w3;
    delete w4;
    return 0;
}
