/*
    Copyright 2015 Elvis Angelaccio <elvis.angelaccio@kde.org>

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

#ifndef KNEWPASSWORDWIDGETTEST_H
#define KNEWPASSWORDWIDGETTEST_H

#include <QObject>

class KNewPasswordWidgetTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void testEmptyPasswordAllowed();
    void testEmptyPasswordNotAllowed();
    void testPasswordTooShort();
    void testPasswordMatch();
    void testPasswordNotVerified();
    void testWeakPassword();
    void testStrongPassword();
    void testReasonablePasswordLength();
    void testPasswordStrengthWarningLevel();
    void testNoWarningColorBeforeMismatch();
    void testWarningColorIfMismatch();
    void testWarningColorPostMatch();
    void disablingWidgetShouldUseDisabledPalette();
    void disablingParentShouldUseDisabledPalette();
    void disablingRevealPasswordShouldHideVisibilityAction();
};

#endif
