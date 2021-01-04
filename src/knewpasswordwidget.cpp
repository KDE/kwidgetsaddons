// vi: ts=8 sts=4 sw=4
/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 1998 Pietro Iglio <iglio@fub.it>
    SPDX-FileCopyrightText: 1999, 2000 Geert Jansen <jansen@kde.org>
    SPDX-FileCopyrightText: 2004, 2005 Andrew Coles <andrew_coles@yahoo.co.uk>
    SPDX-FileCopyrightText: 2007 Michaël Larouche <larouche@kde.org>
    SPDX-FileCopyrightText: 2009 Christoph Feck <cfeck@kde.org>
    SPDX-FileCopyrightText: 2015 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: LGPL-2.0-only
*/

#include "knewpasswordwidget.h"
#include "ui_knewpasswordwidget.h"

class KNewPasswordWidgetPrivate
{
    Q_DECLARE_TR_FUNCTIONS(KNewPasswordWidget)

public:
    KNewPasswordWidgetPrivate(KNewPasswordWidget *parent)
        : q(parent)
    {}

    void init();
    void _k_passwordChanged();
    void _k_toggleEchoMode();
    int effectivePasswordLength(const QString &password);
    void updatePasswordStatus(KNewPasswordWidget::PasswordStatus status);

    KNewPasswordWidget *const q;

    KNewPasswordWidget::PasswordStatus passwordStatus = KNewPasswordWidget::WeakPassword;
    int minimumPasswordLength = 0;
    int passwordStrengthWarningLevel = 1;
    int reasonablePasswordLength = 8;

    QAction *toggleEchoModeAction = nullptr;
    QColor backgroundWarningColor;
    QColor defaultBackgroundColor;

    Ui::KNewPasswordWidget ui;
};

void KNewPasswordWidgetPrivate::init()
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

    QObject::connect(ui.linePassword, SIGNAL(echoModeChanged(QLineEdit::EchoMode)), q, SLOT(_k_toggleEchoMode()));

    QObject::connect(ui.linePassword, SIGNAL(passwordChanged(QString)), q, SLOT(_k_passwordChanged()));
    QObject::connect(ui.lineVerifyPassword, SIGNAL(textChanged(QString)), q, SLOT(_k_passwordChanged()));

    defaultBackgroundColor = q->palette().color(QPalette::Active, QPalette::Base);
    backgroundWarningColor = defaultBackgroundColor;

    _k_passwordChanged();
}

void KNewPasswordWidgetPrivate::_k_passwordChanged()
{
    const QString password = ui.linePassword->password();
    const QString verification = ui.lineVerifyPassword->text();
    const bool match = (password == verification);
    const bool partialMatch = password.startsWith(verification);
    const int minPasswordLength = q->minimumPasswordLength();

    QPalette palette = q->palette();
    palette.setColor(QPalette::Active, QPalette::Base, (match || partialMatch) ? defaultBackgroundColor : backgroundWarningColor);
    ui.lineVerifyPassword->setPalette(palette);

    // Password strength calculator
    int pwstrength = (20 * ui.linePassword->password().length() + 80 * effectivePasswordLength(ui.linePassword->password())) / qMax(reasonablePasswordLength, 2);
    ui.strengthBar->setValue(qBound(0, pwstrength, 100));

    // update the current password status
    if (match || ui.lineVerifyPassword->isHidden()) {
        if (!q->allowEmptyPasswords() && ui.linePassword->password().isEmpty()) {
            updatePasswordStatus(KNewPasswordWidget::EmptyPasswordNotAllowed);
        } else if (ui.linePassword->password().length() < minPasswordLength) {
            updatePasswordStatus(KNewPasswordWidget::PasswordTooShort);
        } else if (ui.strengthBar && ui.strengthBar->value() < passwordStrengthWarningLevel) {
            updatePasswordStatus(KNewPasswordWidget::WeakPassword);
        } else {
            updatePasswordStatus(KNewPasswordWidget::StrongPassword);
        }
    } else {
        updatePasswordStatus(KNewPasswordWidget::PasswordNotVerified);
    }
}

void KNewPasswordWidgetPrivate::_k_toggleEchoMode()
{
    if (ui.linePassword->lineEdit()->echoMode() == QLineEdit::Normal) {
        ui.lineVerifyPassword->hide();
        ui.labelVerifyPassword->hide();
    } else if (ui.linePassword->lineEdit()->echoMode() == QLineEdit::Password) {
        ui.lineVerifyPassword->show();
        ui.labelVerifyPassword->show();
    }
    _k_passwordChanged();
}

int KNewPasswordWidgetPrivate::effectivePasswordLength(const QString &password)
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

void KNewPasswordWidgetPrivate::updatePasswordStatus(KNewPasswordWidget::PasswordStatus status)
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

KNewPasswordWidget::~KNewPasswordWidget() = default;

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
    return d->ui.linePassword->lineEdit()->maxLength();
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

bool KNewPasswordWidget::isRevealPasswordAvailable() const
{
    return d->ui.linePassword->isRevealPasswordAvailable();
}

QString KNewPasswordWidget::password() const
{
    return d->ui.linePassword->password();
}

void KNewPasswordWidget::setAllowEmptyPasswords(bool allowed)
{
    setMinimumPasswordLength(allowed ? 0 : 1);
    d->_k_passwordChanged();
}

void KNewPasswordWidget::setMinimumPasswordLength(int minLength)
{
    d->minimumPasswordLength = minLength;
    d->_k_passwordChanged();
}

void KNewPasswordWidget::setMaximumPasswordLength(int maxLength)
{
    if (maxLength < minimumPasswordLength()) {
        maxLength = minimumPasswordLength();
    }

    d->ui.linePassword->lineEdit()->setMaxLength(maxLength);
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

void KNewPasswordWidget::setRevealPasswordAvailable(bool reveal)
{
    d->ui.linePassword->setRevealPasswordAvailable(reveal);
}

#include "moc_knewpasswordwidget.cpp"
