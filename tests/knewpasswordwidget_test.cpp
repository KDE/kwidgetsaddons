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

#include "knewpasswordwidget_test.h"

#include <QApplication>
#include <QCheckBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

#include <KNewPasswordWidget>
#include <KMessageBox>

// Doxygen will generate code snippets from this file.
// We can't use i18n() here, but we want it to show up in the apidox.
#define i18n QStringLiteral

MyPasswordDialog::MyPasswordDialog(QWidget *parent)
    : QDialog(parent)
{
    m_passwordWidget = new KNewPasswordWidget(this);
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    m_passwordWidget->setMinimumPasswordLength(6);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(m_passwordWidget);
    layout->addWidget(new QCheckBox(QStringLiteral("A checkbox"), this));
    layout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

//! [accept_custom_dialog]
void MyPasswordDialog::accept()
{
    switch (m_passwordWidget->passwordStatus()) {
    case KNewPasswordWidget::WeakPassword:
    case KNewPasswordWidget::StrongPassword:
        QDialog::accept();
        break;
    case KNewPasswordWidget::PasswordNotVerified:
        KMessageBox::error(nullptr, i18n("The chosen password does not match the given verification password."));
        break;
    case KNewPasswordWidget::EmptyPasswordNotAllowed:
        KMessageBox::error(nullptr, i18n("The chosen password cannot be empty."));
        break;
    case KNewPasswordWidget::PasswordTooShort:
        KMessageBox::error(nullptr, i18n("The chosen password is too short."));
        break;
    }
}
//! [accept_custom_dialog]

//! [update_custom_dialog]
void MyPasswordDialog::slotPasswordStatusChanged()
{
    // You may want to extend this switch with more cases,
    // in order to warn the user about all the possible password issues.
    switch (m_passwordWidget->passwordStatus()) {
    case KNewPasswordWidget::WeakPassword:
    case KNewPasswordWidget::StrongPassword:
        m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        break;
    default:
        m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        break;
    }
}
//! [update_custom_dialog]

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    MyPasswordDialog dialog;
    QObject::connect(&dialog, &QDialog::finished, &app, &QCoreApplication::quit);
    dialog.show();

    return app.exec();
}

