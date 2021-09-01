/*
    SPDX-FileCopyrightText: 2009 Michael Leupold <lemma@confuego.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "kmodifierkeystatusprovider_p.h"

KModifierKeyStatusProvider::KModifierKeyStatusProvider()
    : QObject(nullptr)
{
}

KModifierKeyStatusProvider::~KModifierKeyStatusProvider() = default;

bool KModifierKeyStatusProvider::setKeyLatched(Qt::Key key, bool latched)
{
    Q_UNUSED(key);
    Q_UNUSED(latched);
    return false;
}

bool KModifierKeyStatusProvider::setKeyLocked(Qt::Key key, bool locked)
{
    Q_UNUSED(key);
    Q_UNUSED(locked);
    return false;
}

bool KModifierKeyStatusProvider::isFlagSet(Qt::Key key, ModifierFlag flag) const
{
    auto it = std::find_if(m_modifierStates.cbegin(), m_modifierStates.cend(), [=](const ModifierKeyInfo &info) {
        return key == info.modKey;
    });

    return it != m_modifierStates.cend() ? it->modFlags & flag : false;
}

bool KModifierKeyStatusProvider::isKeyPressed(Qt::Key key) const
{
    return isFlagSet(key, Pressed);
}

bool KModifierKeyStatusProvider::isKeyLatched(Qt::Key key) const
{
    return isFlagSet(key, Latched);
}

bool KModifierKeyStatusProvider::isKeyLocked(Qt::Key key) const
{
    return isFlagSet(key, Locked);
}

bool KModifierKeyStatusProvider::isButtonPressed(Qt::MouseButton button) const
{
    auto it = std::find_if(m_MouseBtnStates.cbegin(), m_MouseBtnStates.cend(), [=](const MouseButtonInfo &info) {
        return button == info.mouseButton;
    });

    return it != m_MouseBtnStates.cend() ? it->state : false;
}

bool KModifierKeyStatusProvider::knowsKey(Qt::Key key) const
{
    auto it = std::find_if(m_modifierStates.cbegin(), m_modifierStates.cend(), [=](const ModifierKeyInfo &info) {
        return key == info.modKey;
    });

    return it != m_modifierStates.cend();
}

void KModifierKeyStatusProvider::stateUpdated(const ModifierKeyInfo &newInfo)
{
    auto key = newInfo.modKey;
    auto newFlags = newInfo.modFlags;

    auto it = std::find_if(m_modifierStates.begin(), m_modifierStates.end(), [=](const ModifierKeyInfo &info) {
        return key == info.modKey;
    });

    Q_ASSERT(it != m_modifierStates.cend());

    auto &flags = it->modFlags;

    if (newFlags != flags) {
        const auto difference = (newFlags ^ flags);
        flags = newFlags;
        if (difference & Pressed) {
            Q_EMIT keyPressed(key, newFlags & Pressed);
        }
        if (difference & Latched) {
            Q_EMIT keyLatched(key, newFlags & Latched);
        }
        if (difference & Locked) {
            Q_EMIT keyLocked(key, newFlags & Locked);
        }
    }
}
