/*
    SPDX-FileCopyrightText: 2009 Michael Leupold <lemma@confuego.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "kmodifierkeystatus.h"
#include "kmodifierkeystatusprovider_p.h"
#include <loggingcategory.h>

#include <QGuiApplication>
#include <QPluginLoader>

KModifierKeyStatusProvider *createProvider()
{
    QPluginLoader loader(QStringLiteral("kf5/kwidgetsaddons/kmodifierkey/kmodifierkey_") + qGuiApp->platformName());
    auto instance = dynamic_cast<KModifierKeyStatusProvider *>(loader.instance());
    if (instance) {
        return instance;
    }
    qCWarning(KWidgetsAddonsLog) << "Error: could not load plugin for platform" << loader.fileName() << "error:" << loader.errorString() << loader.instance();
    return new KModifierKeyStatusProvider;
}

KModifierKeyStatus::KModifierKeyStatus(QObject *parent)
    : QObject(parent)
    , m_provider(createProvider())
{
    connect(m_provider.get(), &KModifierKeyStatusProvider::keyPressed, this, &KModifierKeyStatus::keyPressed);
    connect(m_provider.get(), &KModifierKeyStatusProvider::keyLatched, this, &KModifierKeyStatus::keyLatched);
    connect(m_provider.get(), &KModifierKeyStatusProvider::keyLocked, this, &KModifierKeyStatus::keyLocked);
    connect(m_provider.get(), &KModifierKeyStatusProvider::buttonPressed, this, &KModifierKeyStatus::buttonPressed);
    connect(m_provider.get(), &KModifierKeyStatusProvider::keyAdded, this, &KModifierKeyStatus::keyAdded);
    connect(m_provider.get(), &KModifierKeyStatusProvider::keyRemoved, this, &KModifierKeyStatus::keyRemoved);
}

KModifierKeyStatus::~KModifierKeyStatus()
{
}

bool KModifierKeyStatus::knowsKey(Qt::Key key) const
{
    return m_provider->knowsKey(key);
}

const QList<Qt::Key> KModifierKeyStatus::knownKeys() const
{
    return m_provider->knownKeys();
}

bool KModifierKeyStatus::isKeyPressed(Qt::Key key) const
{
    return m_provider->isKeyPressed(key);
}

bool KModifierKeyStatus::isKeyLatched(Qt::Key key) const
{
    return m_provider->isKeyLatched(key);
}

bool KModifierKeyStatus::setKeyLatched(Qt::Key key, bool latched)
{
    return m_provider->setKeyLatched(key, latched);
}

bool KModifierKeyStatus::isKeyLocked(Qt::Key key) const
{
    return m_provider->isKeyLocked(key);
}

bool KModifierKeyStatus::setKeyLocked(Qt::Key key, bool locked)
{
    return m_provider->setKeyLocked(key, locked);
}

bool KModifierKeyStatus::isButtonPressed(Qt::MouseButton button) const
{
    return m_provider->isButtonPressed(button);
}
