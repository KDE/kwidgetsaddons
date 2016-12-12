/*
    Copyright (c) 2016 Elvis Angelaccio <elvis.angelaccio@kde.org>

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

#ifndef KNEWPASSWORDWIDGET_TEST_H
#define KNEWPASSWORDWIDGET_TEST_H

#include <QDialog>

class QDialogButtonBox;

class KNewPasswordWidget;

class MyPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    MyPasswordDialog(QWidget *parent = Q_NULLPTR);

    void accept() Q_DECL_OVERRIDE;

private Q_SLOTS:
    void slotPasswordStatusChanged();

private:
    KNewPasswordWidget *m_passwordWidget;
    QDialogButtonBox *m_buttonBox;
};

#endif
