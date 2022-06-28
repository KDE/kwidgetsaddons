/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 2012 David Faure <faure+bluesystems@kde.org>

    SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KMESSAGEBOXDONTASKAGAININTERFACE_H
#define KMESSAGEBOXDONTASKAGAININTERFACE_H

#include "kmessagebox.h"
#include <qmetatype.h>

/**
 * @internal
 * Used internally by KMessageBox, implemented by plugin, for dynamic dependency on KConfig.
 */
class KMessageBoxDontAskAgainInterface
{
public:
    KMessageBoxDontAskAgainInterface()
    {
    }
    virtual ~KMessageBoxDontAskAgainInterface()
    {
    }

#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 100)
    virtual bool shouldBeShownYesNo(const QString &dontShowAgainName, KMessageBox::ButtonCode &result) = 0;
#else
    virtual bool shouldBeShownTwoActions(const QString &dontShowAgainName, KMessageBox::ButtonCode &result) = 0;
#endif
    virtual bool shouldBeShownContinue(const QString &dontShowAgainName) = 0;
#if KWIDGETSADDONS_BUILD_DEPRECATED_SINCE(5, 100)
    virtual void saveDontShowAgainYesNo(const QString &dontShowAgainName, KMessageBox::ButtonCode result) = 0;
#else
    virtual void saveDontShowAgainTwoActions(const QString &dontShowAgainName, KMessageBox::ButtonCode result) = 0;
#endif
    virtual void saveDontShowAgainContinue(const QString &dontShowAgainName) = 0;
    virtual void enableAllMessages() = 0;
    virtual void enableMessage(const QString &dontShowAgainName) = 0;
    virtual void setConfig(KConfig *) = 0;
};

Q_DECLARE_METATYPE(KMessageBoxDontAskAgainInterface *)

#define KMESSAGEBOXDONTASKAGAIN_PROPERTY "KMessageBoxDontAskAgain"

#endif
