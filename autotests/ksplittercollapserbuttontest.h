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

#ifndef KSPLITTERCOLLAPSERBUTTONTEST_H
#define KSPLITTERCOLLAPSERBUTTONTEST_H

#include <QWidget>
class QSplitter;
class QTextEdit;

class TestSplitter : public QWidget
{
    Q_OBJECT
public:
    explicit TestSplitter(QWidget *parent = 0);
    QSplitter *splitter;
    QTextEdit *edit1;
    QTextEdit *edit2;
};

class KSplitterCollapserButtonTest : public QObject
{
    Q_OBJECT
public:
    explicit KSplitterCollapserButtonTest(QObject *parent = 0);
    ~KSplitterCollapserButtonTest();

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldCollapseWhenClickOnButton();
    void shouldRestoreCorrectPosition();
    void shouldRestoreCorrectPositionForFirstWidget();
    void shouldTestVerticalSplitterFirstWidget();
    void shouldTestVerticalSplitterSecondWidget();
    void shouldBeVisible_data();
    void shouldBeVisible();
    void shouldBeVisibleWhenMovingHandle_data();
    void shouldBeVisibleWhenMovingHandle();
};

#endif // KSPLITTERCOLLAPSERBUTTONTEST_H
