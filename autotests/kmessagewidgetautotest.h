/*
    Copyright 2014 Dominik Haumann <dhaumann@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef KMESSAGEWIDGETAUTOTEST_H
#define KMESSAGEWIDGETAUTOTEST_H

#include <QObject>

class KMessageWidgetTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testAnimationSignals();
    void testShowOnVisible();
    void testHideOnInvisible();
    void testOverlappingShowAndHide_data();
    void testOverlappingShowAndHide();
    void testOverlappingHideAndShow_data();
    void testOverlappingHideAndShow();
    void testOverlappingDoubleShow_data();
    void testOverlappingDoubleShow();
    void testOverlappingDoubleHide_data();
    void testOverlappingDoubleHide();
    void testHideWithNotYetShownParent();
    void testNonAnimatedShowAfterAnimatedHide();
};

#endif
