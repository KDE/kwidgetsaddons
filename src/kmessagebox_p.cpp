/*  This file is part of the KDE libraries
 *  Copyright 2012 David Faure <faure+bluesystems@kde.org>
 *  Copyright 2013 Aurélien Gâteau <agateau@kde.org>
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2 of the License or ( at
 *  your option ) version 3 or, at the discretion of KDE e.V. ( which shall
 *  act as a proxy as in section 14 of the GPLv3 ), any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */
#include "kmessagebox_p.h"

#include <QCoreApplication>
#include <QDebug>
#include <QPluginLoader>
#include <QSettings>
#include <QStandardPaths>
#include <QVariant>

namespace KMessageBox
{

class KMessageBoxDontAskAgainQSettingsStorage : public KMessageBoxDontAskAgainInterface
{
public:
    KMessageBoxDontAskAgainQSettingsStorage()
    {
        m_filePath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + QCoreApplication::instance()->applicationName() + QStringLiteral(".kmessagebox");
        QSettings s(m_filePath, QSettings::IniFormat);
        foreach (const QString &key, s.allKeys()) {
            m_saved.insert(key, static_cast<KMessageBox::ButtonCode>(s.value(key).toInt()));
        }
    }

    virtual ~KMessageBoxDontAskAgainQSettingsStorage()
    {
        QSettings s(m_filePath, QSettings::IniFormat);
        for (auto it = m_saved.constBegin(); it != m_saved.constEnd(); ++it) {
            s.setValue(it.key(), static_cast<int>(it.value()));
        }
    }

    bool shouldBeShownYesNo(const QString &dontShowAgainName, KMessageBox::ButtonCode &result) Q_DECL_OVERRIDE
    {
        KMessageBox::ButtonCode code = m_saved.value(dontShowAgainName, KMessageBox::ButtonCode(0));
        if (code == KMessageBox::Yes || code == KMessageBox::No) {
            result = code;
            return false;
        }
        return true;
    }
    bool shouldBeShownContinue(const QString &dontShowAgainName) Q_DECL_OVERRIDE
    {
        KMessageBox::ButtonCode code = m_saved.value(dontShowAgainName, KMessageBox::Yes);
        return code == KMessageBox::Yes;
    }
    void saveDontShowAgainYesNo(const QString &dontShowAgainName, KMessageBox::ButtonCode result) Q_DECL_OVERRIDE
    {
        m_saved[dontShowAgainName] = result;
    }
    void saveDontShowAgainContinue(const QString &dontShowAgainName) Q_DECL_OVERRIDE
    {
        m_saved[dontShowAgainName] = KMessageBox::No;
    }
    void enableAllMessages() Q_DECL_OVERRIDE
    {
        m_saved.clear();
    }
    void enableMessage(const QString &dontShowAgainName) Q_DECL_OVERRIDE
    {
        m_saved.remove(dontShowAgainName);
    }
    void setConfig(KConfig *) Q_DECL_OVERRIDE
    {
        qWarning() << "Using QSettings based KMessageBoxDontAskAgainInterface. KMessageBox::setDontShowAgainConfig ignored";
    }

private:
    QString m_filePath;
    QHash<QString, KMessageBox::ButtonCode> m_saved;
};

class KMessageBoxNotifyDummy : public KMessageBoxNotifyInterface
{
public:
    void sendNotification(QMessageBox::Icon /*notificationType*/, const QString &/*message*/, QWidget * /*parent*/) Q_DECL_OVERRIDE {}
};

Q_GLOBAL_STATIC(KMessageBoxDontAskAgainQSettingsStorage, s_defaultDontAskAgainInterface)
Q_GLOBAL_STATIC(KMessageBoxNotifyDummy, s_defaultNotifyInterface)

static KMessageBoxDontAskAgainInterface *s_dontAskAgainInterface = 0;
static KMessageBoxNotifyInterface *s_notifyInterface = 0;

static void loadKMessageBoxPlugin()
{
    static bool triedLoadingPlugin = false;
    if (!triedLoadingPlugin) {
        triedLoadingPlugin = true;

        QPluginLoader lib(QStringLiteral("kf5/FrameworkIntegrationPlugin"));
        QObject *rootObj = lib.instance();
        if (rootObj) {
            s_dontAskAgainInterface = rootObj->property(KMESSAGEBOXDONTASKAGAIN_PROPERTY).value<KMessageBoxDontAskAgainInterface *>();
            s_notifyInterface = rootObj->property(KMESSAGEBOXNOTIFY_PROPERTY).value<KMessageBoxNotifyInterface *>();
        }
    }
    if (!s_dontAskAgainInterface) {
        s_dontAskAgainInterface = s_defaultDontAskAgainInterface;
    }
    if (!s_notifyInterface) {
        s_notifyInterface = s_defaultNotifyInterface;
    }
}

KMessageBoxDontAskAgainInterface *dontAskAgainInterface()
{
    if (!s_dontAskAgainInterface) {
        loadKMessageBoxPlugin();
    }
    return s_dontAskAgainInterface;
}

KMessageBoxNotifyInterface *notifyInterface()
{
    if (!s_notifyInterface) {
        loadKMessageBoxPlugin();
    }
    return s_notifyInterface;
}

void setDontShowAgainInterface(KMessageBoxDontAskAgainInterface *dontAskAgainInterface)
{
    Q_ASSERT(dontAskAgainInterface != 0);
    s_dontAskAgainInterface = dontAskAgainInterface;
}

void setNotifyInterface(KMessageBoxNotifyInterface *notifyInterface)
{
    Q_ASSERT(notifyInterface != 0);
    s_notifyInterface = notifyInterface;
}

} // KMessageBox namespace
