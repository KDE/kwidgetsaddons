/*
    SPDX-FileCopyrightText: 2017 Montel Laurent <montel@kde.org>
    SPDX-FileCopyrightText: 2015 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KPASSWORDLINEEDIT_H
#define KPASSWORDLINEEDIT_H

#include <QLineEdit>
#include <QWidget>
#include <kwidgetsaddons_export.h>
#include <memory>
class QAction;

/**
 * @class KPasswordLineEdit kpasswordlineedit.h KPasswordLineEdit
 *
 * A lineedit which allows to display password
 *
 * \section usage Usage Example
 *
 * Get password
 *
 * \code
 * KPasswordLineEdit *passwordLineEdit = new KPasswordLineEdit(parent);
 * QString password = passwordLineEdit->password();
 * \endcode
 *
 * @author Laurent Montel <montel@kde.org>
 * @since 5.37
 */
class KWIDGETSADDONS_EXPORT KPasswordLineEdit : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(bool clearButtonEnabled READ isClearButtonEnabled WRITE setClearButtonEnabled)
    Q_PROPERTY(QLineEdit::EchoMode echoMode READ echoMode WRITE setEchoMode NOTIFY echoModeChanged)
public:
    /**
     * Constructs a lineedit password widget.
     * @since 5.37
     *
     * @param parent Passed to lower level constructor.
     */
    explicit KPasswordLineEdit(QWidget *parent = nullptr);
    /**
     * Destructs the lineedit password widget.
     */
    ~KPasswordLineEdit();

    /**
     * Assign password
     */
    void setPassword(const QString &password);

    /**
     * Returns the password entered.
     */
    QString password() const;

    /**
     * Clear text
     */
    void clear();

    /**
     * Show/hide clear button (false by default)
     */
    void setClearButtonEnabled(bool clear);

    /**
     * Inform if we show or not clear button
     */
    bool isClearButtonEnabled() const;

    /**
     * Change echo mode (QLineEdit::Password by default)
     */
    void setEchoMode(QLineEdit::EchoMode mode);

    /**
     * Return echo mode
     */
    QLineEdit::EchoMode echoMode() const;

    /**
     * Whether to show the visibility trailing action in the line edit.
     * Default is true. This can be used to honor the lineedit_reveal_password
     * kiosk key, for example:
     * \code
     * passwordLineEdit.setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));
     * \endcode
     */
    void setRevealPasswordAvailable(bool reveal);

    /**
     * Whether the visibility trailing action in the line edit is visible.
     */
    bool isRevealPasswordAvailable() const;

    /**
     * @internal
     * Returns the QAction
     */
    QAction *toggleEchoModeAction() const;

    /**
     * Returns the lineedit widget.
     */
    QLineEdit *lineEdit() const;

Q_SIGNALS:
    /**
     * When we click on visibility icon echo mode is switched between Normal echo mode and Password echo mode
     */
    void echoModeChanged(QLineEdit::EchoMode echoMode);
    void passwordChanged(const QString &password);

private:
    std::unique_ptr<class KPasswordLineEditPrivate> const d;
};

#endif
