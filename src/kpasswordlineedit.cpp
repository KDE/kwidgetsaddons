/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>
   Copyright (C) 2015 Elvis Angelaccio <elvis.angelaccio@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "kpasswordlineedit.h"

#include <QIcon>
#include <QAction>
#include <QHBoxLayout>


class KPasswordLineEditPrivate
{
public:
    KPasswordLineEditPrivate(KPasswordLineEdit *qq)
        : q(qq)
    {
    }
    void initialize();
    void _k_echoModeToggled();
    void _k_showToggleEchoModeAction(const QString &text);

    QIcon passwordIcon;
    QIcon visibleIcon;

    QLineEdit *passwordLineEdit = nullptr;
    QAction *toggleEchoModeAction = nullptr;
    bool isToggleEchoModeAvailable = true;
    bool revealPasswordAvailable = true;
    KPasswordLineEdit *q;
};

void KPasswordLineEditPrivate::initialize()
{
    QIcon visibilityIcon = QIcon::fromTheme(QStringLiteral("visibility"), QIcon(QStringLiteral(":/icons/visibility.svg")));
    toggleEchoModeAction = passwordLineEdit->addAction(visibilityIcon, QLineEdit::TrailingPosition);
    toggleEchoModeAction->setObjectName(QStringLiteral("visibilityAction"));
    toggleEchoModeAction->setVisible(false);
    toggleEchoModeAction->setToolTip(QObject::tr("Change the visibility of the password", "@info:tooltip"));
    q->connect(toggleEchoModeAction, &QAction::triggered, q, [this]() {_k_echoModeToggled();});
    q->connect(passwordLineEdit, &QLineEdit::textChanged, q, [this](const QString &str) {_k_showToggleEchoModeAction(str);});
}

void KPasswordLineEditPrivate::_k_showToggleEchoModeAction(const QString &text)
{
    if (revealPasswordAvailable) {
        toggleEchoModeAction->setVisible(isToggleEchoModeAvailable && (passwordLineEdit->echoMode() == QLineEdit::Normal || !text.isEmpty()));
    } else {
        toggleEchoModeAction->setVisible(false);
    }
}

void KPasswordLineEditPrivate::_k_echoModeToggled()
{
    if (passwordLineEdit->echoMode() == QLineEdit::Password) {
        passwordLineEdit->setEchoMode(QLineEdit::Normal);
        if (passwordIcon.isNull()) {
            passwordIcon = QIcon::fromTheme(QStringLiteral("hint"), QIcon(QStringLiteral(":/icons/hint.svg")));
        }
        toggleEchoModeAction->setIcon(passwordIcon);
    } else if (passwordLineEdit->echoMode() == QLineEdit::Normal) {
        if (visibleIcon.isNull()) {
            visibleIcon = QIcon::fromTheme(QStringLiteral("visibility"), QIcon(QStringLiteral(":/icons/visibility.svg")));
        }
        passwordLineEdit->setEchoMode(QLineEdit::Password);
        toggleEchoModeAction->setIcon(visibleIcon);
    }
    Q_EMIT q->echoModeChanged(passwordLineEdit->echoMode());
}

KPasswordLineEdit::KPasswordLineEdit(QWidget *parent)
    : QWidget(parent)
    , d(new KPasswordLineEditPrivate(this))
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);
    d->passwordLineEdit = new QLineEdit(this);
    d->passwordLineEdit->setObjectName(QStringLiteral("passwordlineedit"));
    d->passwordLineEdit->setEchoMode(QLineEdit::Password);
    connect(d->passwordLineEdit, &QLineEdit::textChanged, this, &KPasswordLineEdit::passwordChanged);
    setFocusProxy(d->passwordLineEdit);
    setFocusPolicy(d->passwordLineEdit->focusPolicy());
    mainLayout->addWidget(d->passwordLineEdit);
    d->initialize();
}

KPasswordLineEdit::~KPasswordLineEdit()
{
    delete d;
}

void KPasswordLineEdit::setPassword(const QString &password)
{
    if (d->passwordLineEdit->text() != password) {
        d->isToggleEchoModeAvailable = password.isEmpty();
        d->passwordLineEdit->setText(password);
        Q_EMIT passwordChanged(password);
    }
}

QString KPasswordLineEdit::password() const
{
    return d->passwordLineEdit->text();
}

void KPasswordLineEdit::clear()
{
    d->passwordLineEdit->clear();
}

void KPasswordLineEdit::setClearButtonEnabled(bool clear)
{
    d->passwordLineEdit->setClearButtonEnabled(clear);
}

bool KPasswordLineEdit::isClearButtonEnabled() const
{
    return d->passwordLineEdit->isClearButtonEnabled();
}

void KPasswordLineEdit::setEchoMode(QLineEdit::EchoMode mode)
{
    d->passwordLineEdit->setEchoMode(mode);
}

QLineEdit::EchoMode KPasswordLineEdit::echoMode() const
{
    return d->passwordLineEdit->echoMode();
}

QLineEdit *KPasswordLineEdit::lineEdit() const
{
    return d->passwordLineEdit;
}

void KPasswordLineEdit::setRevealPasswordAvailable(bool reveal)
{
    d->revealPasswordAvailable = reveal;
    d->_k_showToggleEchoModeAction(password());
}

bool KPasswordLineEdit::isRevealPasswordAvailable() const
{
    return d->revealPasswordAvailable;
}

QAction *KPasswordLineEdit::toggleEchoModeAction() const
{
    return d->toggleEchoModeAction;
}

#include "moc_kpasswordlineedit.cpp"
