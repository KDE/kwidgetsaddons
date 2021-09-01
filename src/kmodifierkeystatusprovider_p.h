/*
    SPDX-FileCopyrightText: 2009 Michael Leupold <lemma@confuego.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KMODIFIERKEYSTATUSPROVIDER_P_H
#define KMODIFIERKEYSTATUSPROVIDER_P_H

#include "kwidgetsaddons_export.h"

#include <QHash>
#include <QObject>
#include <QSharedData>

/**
 * Background class that implements the behaviour of KModifierKeyStatus for
 * the different supported platforms.
 * @internal
 */
class KWIDGETSADDONS_EXPORT KModifierKeyStatusProvider : public QObject
{
    Q_OBJECT

public:
    enum ModifierFlag {
        Nothing = 0x0,
        Pressed = 0x1,
        Latched = 0x2,
        Locked = 0x4,
    };
    Q_ENUM(ModifierFlag);
    Q_DECLARE_FLAGS(ModifierFlags, ModifierFlag)

    KModifierKeyStatusProvider();
    ~KModifierKeyStatusProvider() override;

    bool isFlagSet(Qt::Key key, ModifierFlag flag) const;

    /**
     * Detect if a key is pressed.
     * @param key Modifier key to query
     * @return true if the key is pressed, false if it isn't.
     */
    bool isKeyPressed(Qt::Key key) const;

    /**
     * Detect if a key is latched.
     * @param key Modifier key to query
     * @return true if the key is latched, false if it isn't.
     */
    bool isKeyLatched(Qt::Key key) const;

    /**
     * Set the latched state of a key.
     * @param key Modifier to set the latched state for
     * @param latched true to latch the key, false to unlatch it
     * @return true if the key is known, false else
     */
    virtual bool setKeyLatched(Qt::Key key, bool latched);

    /**
     * Detect if a key is locked.
     * @param key Modifier key to query
     * @return true if the key is locked, false if it isn't.
     */
    bool isKeyLocked(Qt::Key key) const;

    /**
     * Set the locked state of a key.
     * @param key Modifier to set the locked state for
     * @param latched true to lock the key, false to unlock it
     * @return true if the key is known, false else
     */
    virtual bool setKeyLocked(Qt::Key key, bool locked);

    /**
     * Check if a mouse button is pressed.
     * @param button Mouse button to check
     * @return true if pressed, false else
     */
    bool isButtonPressed(Qt::MouseButton button) const;

    /**
     * Check if a key is known/can be queried
     * @param key Modifier key to check
     * @return true if the key is known, false if it isn't.
     */
    bool knowsKey(Qt::Key key) const;

Q_SIGNALS:
    void keyLatched(Qt::Key key, bool state);
    void keyLocked(Qt::Key key, bool state);
    void keyPressed(Qt::Key key, bool state);
    void buttonPressed(Qt::MouseButton button, bool state);
    void keyAdded(Qt::Key key);
    void keyRemoved(Qt::Key key);

protected:
    struct ModifierKeyInfo {
        Qt::Key modKey = Qt::Key_unknown;
        ModifierFlags modFlags = Nothing;
    };

    void stateUpdated(const ModifierKeyInfo &newInfo);

    // The state of each known modifier
    std::vector<ModifierKeyInfo> m_modifierStates;

    // the state of each known mouse button
    QHash<Qt::MouseButton, bool> m_buttonStates;
};

Q_DECLARE_INTERFACE(KModifierKeyStatusProvider, "org.kde.kwidgetsaddons.KModifierKeyStatusProvider")
Q_DECLARE_OPERATORS_FOR_FLAGS(KModifierKeyStatusProvider::ModifierFlags)

#endif
