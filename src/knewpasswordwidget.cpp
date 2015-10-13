// vi: ts=8 sts=4 sw=4
/* This file is part of the KDE libraries
   Copyright (C) 1998 Pietro Iglio <iglio@fub.it>
   Copyright (C) 1999,2000 Geert Jansen <jansen@kde.org>
   Copyright (C) 2004,2005 Andrew Coles <andrew_coles@yahoo.co.uk>
   Copyright (C) 2007 Michaël Larouche <larouche@kde.org>
   Copyright (C) 2009 Christoph Feck <cfeck@kde.org>
   Copyright (C) 2015 Elvis Angelaccio <elvis.angelaccio@kdemail.net>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "knewpasswordwidget.h"
#include "ui_knewpasswordwidget.h"

class KNewPasswordWidget::KNewPasswordWidgetPrivate
{
public:
    KNewPasswordWidgetPrivate(KNewPasswordWidget *parent)
        : q(parent),
          passwordStatus(WeakPassword),
          minimumPasswordLength(0),
          passwordStrengthWarningLevel(1),
          reasonablePasswordLength(8),
          toggleEchoModeAction(Q_NULLPTR)
    {}

    void init();
    void _k_textChanged();
    void _k_toggleEchoMode();
    void _k_showToggleEchoModeAction(const QString &text);
    int effectivePasswordLength(const QString &password);
    void updatePasswordStatus(PasswordStatus status);

    KNewPasswordWidget *q;

    PasswordStatus passwordStatus;
    int minimumPasswordLength;
    int passwordStrengthWarningLevel;
    int reasonablePasswordLength;

    QAction *toggleEchoModeAction;
    QColor backgroundWarningColor;
    QColor defaultBackgroundColor;

    Ui::KNewPasswordWidget ui;
};

void KNewPasswordWidget::KNewPasswordWidgetPrivate::init()
{
    ui.setupUi(q);

    const QString strengthBarWhatsThis(tr("The password strength meter gives an indication of the security "
                                          "of the password you have entered. To improve the strength of the password, try:"
                                          "<ul><li>using a longer password;</li>"
                                          "<li>using a mixture of upper- and lower-case letters;</li>"
                                          "<li>using numbers or symbols, such as #, as well as letters.</li></ul>",
                                          "@info:whatsthis"));
    ui.labelStrengthMeter->setWhatsThis(strengthBarWhatsThis);
    ui.strengthBar->setWhatsThis(strengthBarWhatsThis);

    QIcon visibilityIcon = QIcon::fromTheme(QStringLiteral("visibility"), QIcon(QStringLiteral(":/icons/visibility.svg")));
    toggleEchoModeAction = ui.linePassword->addAction(visibilityIcon, QLineEdit::TrailingPosition);
    toggleEchoModeAction->setVisible(false);
    toggleEchoModeAction->setToolTip(tr("Change the visibility of the password"));
    connect(toggleEchoModeAction, SIGNAL(triggered(bool)), q, SLOT(_k_toggleEchoMode()));

    connect(ui.linePassword, SIGNAL(textChanged(QString)), q, SLOT(_k_showToggleEchoModeAction(QString)));
    connect(ui.linePassword, SIGNAL(textChanged(QString)), q, SLOT(_k_textChanged()));
    connect(ui.lineVerifyPassword, SIGNAL(textChanged(QString)), q, SLOT(_k_textChanged()));

    defaultBackgroundColor = q->palette().color(QPalette::Base);

    _k_textChanged();
}

void KNewPasswordWidget::KNewPasswordWidgetPrivate::_k_textChanged()
{
    const bool match = ui.linePassword->text() == ui.lineVerifyPassword->text();
    const int minPasswordLength = q->minimumPasswordLength();

    QPalette palette = q->palette();
    palette.setColor(QPalette::Base, match ? defaultBackgroundColor : backgroundWarningColor);
    ui.lineVerifyPassword->setPalette(palette);

    // Password strength calculator
    int pwstrength = (20 * ui.linePassword->text().length() + 80 * effectivePasswordLength(ui.linePassword->text())) / qMax(reasonablePasswordLength, 2);
    ui.strengthBar->setValue(qBound(0, pwstrength, 100));

    // update the current password status
    if (match || ui.lineVerifyPassword->isHidden()) {
        if (!q->allowEmptyPasswords() && ui.linePassword->text().isEmpty()) {
            updatePasswordStatus(EmptyPasswordNotAllowed);
        } else if (ui.linePassword->text().length() < minPasswordLength) {
            updatePasswordStatus(PasswordTooShort);
        } else if (ui.strengthBar && ui.strengthBar->value() < passwordStrengthWarningLevel) {
            updatePasswordStatus(WeakPassword);
        } else {
            updatePasswordStatus(StrongPassword);
        }
    } else {
        updatePasswordStatus(PasswordNotVerified);
    }
}

void KNewPasswordWidget::KNewPasswordWidgetPrivate::_k_toggleEchoMode()
{
    if (ui.linePassword->echoMode() == QLineEdit::Password) {
        ui.linePassword->setEchoMode(QLineEdit::Normal);
        ui.lineVerifyPassword->hide();
        ui.labelVerifyPassword->hide();
        toggleEchoModeAction->setIcon(QIcon::fromTheme(QStringLiteral("hint"), QIcon(QStringLiteral(":/icons/hint.svg"))));
    } else if (ui.linePassword->echoMode() == QLineEdit::Normal) {
        ui.linePassword->setEchoMode(QLineEdit::Password);
        ui.lineVerifyPassword->show();
        ui.labelVerifyPassword->show();
        toggleEchoModeAction->setIcon(QIcon::fromTheme(QStringLiteral("visibility"), QIcon(QStringLiteral(":/icons/visibility.svg"))));
    }

    _k_textChanged();
}

void KNewPasswordWidget::KNewPasswordWidgetPrivate::_k_showToggleEchoModeAction(const QString &text)
{
    toggleEchoModeAction->setVisible(!text.isEmpty());
}

int KNewPasswordWidget::KNewPasswordWidgetPrivate::effectivePasswordLength(const QString &password)
{
    enum Category {
        Digit,
        Upper,
        Vowel,
        Consonant,
        Special
    };

    Category previousCategory = Vowel;
    QString vowels(QStringLiteral("aeiou"));
    int count = 0;

    for (int i = 0; i < password.length(); ++i) {
        QChar currentChar = password.at(i);
        if (!password.leftRef(i).contains(currentChar)) {
            Category currentCategory;
            switch (currentChar.category()) {
            case QChar::Letter_Uppercase:
                currentCategory = Upper;
                break;
            case QChar::Letter_Lowercase:
                if (vowels.contains(currentChar)) {
                    currentCategory = Vowel;
                } else {
                    currentCategory = Consonant;
                }
                break;
            case QChar::Number_DecimalDigit:
                currentCategory = Digit;
                break;
            default:
                currentCategory = Special;
                break;
            }
            switch (currentCategory) {
            case Vowel:
                if (previousCategory != Consonant) {
                    ++count;
                }
                break;
            case Consonant:
                if (previousCategory != Vowel) {
                    ++count;
                }
                break;
            default:
                if (previousCategory != currentCategory) {
                    ++count;
                }
                break;
            }
            previousCategory = currentCategory;
        }
    }
    return count;
}

void KNewPasswordWidget::KNewPasswordWidgetPrivate::updatePasswordStatus(PasswordStatus status)
{
    if (passwordStatus == status) {
        return;
    }

    passwordStatus = status;
    emit q->passwordStatusChanged();
}

KNewPasswordWidget::KNewPasswordWidget(QWidget *parent)
    : QWidget(parent), d(new KNewPasswordWidgetPrivate(this))
{
    d->init();
}

KNewPasswordWidget::~KNewPasswordWidget()
{
    delete d;
}

KNewPasswordWidget::PasswordStatus KNewPasswordWidget::passwordStatus() const
{
    return d->passwordStatus;
}

bool KNewPasswordWidget::allowEmptyPasswords() const
{
    return d->minimumPasswordLength == 0;
}

int KNewPasswordWidget::minimumPasswordLength() const
{
    return d->minimumPasswordLength;
}

int KNewPasswordWidget::maximumPasswordLength() const
{
    return d->ui.linePassword->maxLength();
}

int KNewPasswordWidget::reasonablePasswordLength() const
{
    return d->reasonablePasswordLength;
}

int KNewPasswordWidget::passwordStrengthWarningLevel() const
{
    return d->passwordStrengthWarningLevel;
}

QColor KNewPasswordWidget::backgroundWarningColor() const
{
    return d->backgroundWarningColor;
}

bool KNewPasswordWidget::isPasswordStrengthMeterVisible() const
{
    return d->ui.labelStrengthMeter->isVisible() && d->ui.strengthBar->isVisible();
}

QString KNewPasswordWidget::password() const
{
    return d->ui.linePassword->text();
}

void KNewPasswordWidget::setAllowEmptyPasswords(bool allowed)
{
    setMinimumPasswordLength(allowed ? 0 : 1);
    d->_k_textChanged();
}

void KNewPasswordWidget::setMinimumPasswordLength(int minLength)
{
    d->minimumPasswordLength = minLength;
    d->_k_textChanged();
}

void KNewPasswordWidget::setMaximumPasswordLength(int maxLength)
{
    if (maxLength < minimumPasswordLength()) {
        maxLength = minimumPasswordLength();
    }

    d->ui.linePassword->setMaxLength(maxLength);
    d->ui.lineVerifyPassword->setMaxLength(maxLength);
}

// reasonable password length code contributed by Steffen Mthing

void KNewPasswordWidget::setReasonablePasswordLength(int reasonableLength)
{
    d->reasonablePasswordLength = qBound(1, reasonableLength, maximumPasswordLength());
}

void KNewPasswordWidget::setPasswordStrengthWarningLevel(int warningLevel)
{
    d->passwordStrengthWarningLevel = qBound(0, warningLevel, 99);
}

void KNewPasswordWidget::setBackgroundWarningColor(const QColor &color)
{
    d->backgroundWarningColor = color;
    update();
}

void KNewPasswordWidget::setPasswordStrengthMeterVisible(bool visible)
{
    d->ui.labelStrengthMeter->setVisible(visible);
    d->ui.strengthBar->setVisible(visible);
}

#include "moc_knewpasswordwidget.cpp"
