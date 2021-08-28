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
    , p(createProvider())
{
    connect(p.data(), &KModifierKeyStatusProvider::keyPressed, this, &KModifierKeyStatus::keyPressed);
    connect(p.data(), &KModifierKeyStatusProvider::keyLatched, this, &KModifierKeyStatus::keyLatched);
    connect(p.data(), &KModifierKeyStatusProvider::keyLocked, this, &KModifierKeyStatus::keyLocked);
    connect(p.data(), &KModifierKeyStatusProvider::buttonPressed, this, &KModifierKeyStatus::buttonPressed);
    connect(p.data(), &KModifierKeyStatusProvider::keyAdded, this, &KModifierKeyStatus::keyAdded);
    connect(p.data(), &KModifierKeyStatusProvider::keyRemoved, this, &KModifierKeyStatus::keyRemoved);
}

KModifierKeyStatus::~KModifierKeyStatus()
{
}

bool KModifierKeyStatus::knowsKey(Qt::Key key) const
{
    return p->knowsKey(key);
}

const QList<Qt::Key> KModifierKeyStatus::knownKeys() const
{
    return p->knownKeys();
}

bool KModifierKeyStatus::isKeyPressed(Qt::Key key) const
{
    return p->isKeyPressed(key);
}

bool KModifierKeyStatus::isKeyLatched(Qt::Key key) const
{
    return p->isKeyLatched(key);
}

bool KModifierKeyStatus::setKeyLatched(Qt::Key key, bool latched)
{
    return p->setKeyLatched(key, latched);
}

bool KModifierKeyStatus::isKeyLocked(Qt::Key key) const
{
    return p->isKeyLocked(key);
}

bool KModifierKeyStatus::setKeyLocked(Qt::Key key, bool locked)
{
    return p->setKeyLocked(key, locked);
}

bool KModifierKeyStatus::isButtonPressed(Qt::MouseButton button) const
{
    return p->isButtonPressed(button);
}
