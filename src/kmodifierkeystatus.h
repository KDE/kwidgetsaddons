/*
    SPDX-FileCopyrightText: 2009 Michael Leupold <lemma@confuego.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KMODIFIERKEYSTATUS_H
#define KMODIFIERKEYSTATUS_H

#include <kwidgetsaddons_export.h>

#include <QExplicitlySharedDataPointer>
#include <QObject>

class KModifierKeyStatusProvider;

/**
 * @class KModifierKeyStatus kmodifierkeystatus.h KModifierKeyStatus
 *
 * Get information about the state of the keyboard's modifier keys.
 *
 * This class provides cross-platform information about the state of the
 * keyboard's modifier keys and the mouse buttons and allows to change the
 * states as well.
 *
 * It recognizes two states a key can be in:
 * @li @em locked: e.g. caps-locked (a.k.a. toggled)
 * @li @em latched: the key is temporarily locked but will be unlocked upon
 * the next keypress.
 *
 * An application can either query the states synchronously (isKeyLatched(),
 * isKeyLocked()) or connect to KModifierKeyStatus's signals (e.g. keyLatched(),
 * keyLocked()) to be notified about changes.
 *
 * @since 5.87
 */
class KWIDGETSADDONS_EXPORT KModifierKeyStatus : public QObject
{
    Q_OBJECT

public:
    /**
     * Default constructor
     */
    explicit KModifierKeyStatus(QObject *parent = nullptr);

    /**
     * Destructor
     */
    virtual ~KModifierKeyStatus();

    /**
     * Check if a key is known by the underlying window system and can be queried.
     *
     * @param key the key to check
     * @return @c true if the key is available, @c false if it is unknown
     */
    bool knowsKey(Qt::Key key) const;

    /**
     * Get a list of known keys.
     *
     * @return A list of known keys of which states will be reported.
     */
    const QList<Qt::Key> knownKeys() const;

    /**
     * Synchronously check if a key is pressed.
     *
     * @param key the key to check
     * @return @c true if the key is pressed, @c false if the key is not pressed
     * or unknown.
     *
     * @see isKeyLatched, isKeyLocked, keyPressed
     */
    bool isKeyPressed(Qt::Key key) const;

    /**
     * Synchronously check if a key is latched.
     *
     * @param key the key to check
     * @return @c true if the key is latched, @c false if the key is not latched or unknown.
     *
     * @see isKeyPressed, isKeyLocked, keyLatched
     */
    bool isKeyLatched(Qt::Key key) const;

    /**
     * Set the latched state of a key.
     *
     * @param key the key to latch
     * @param latched @c true to latch the key, @c false to unlatch it.
     * @return @c true if the request was sent to the underlying system
     * (doesn't guarantee that the key state was actually changed), otherwise
     * returns @c false if the key is unknown.
     */
    bool setKeyLatched(Qt::Key key, bool latched);

    /**
     * Synchronously check if a key is locked.
     *
     * @param key the key to check
     * @return @c true if the key is locked, @c false if the key is not locked or unknown.
     * @see isKeyPressed(), isKeyLatched(), keyLocked()
     */
    bool isKeyLocked(Qt::Key key) const;

    /**
     * Set the locked state of a key.
     *
     * @param key the key to lock
     * @param latched @c true to lock the key, @c false to unlock it.
     * @return @c true if the request was sent to the underlying system
     * (doesn't guarantee that the key state was actually changed), otherwise
     * returns @c false if the key is unknown.
     */
    bool setKeyLocked(Qt::Key key, bool locked);

    /**
     * Synchronously check if a mouse button is pressed.
     *
     * @param button the mouse button to check
     * @return @c true if the mouse button is pressed, @c false if the mouse button
     * is not pressed or its state is unknown.
     */
    bool isButtonPressed(Qt::MouseButton button) const;

Q_SIGNALS:
    /**
     * Emitted whenever the pressed state of a key changes (key press or key release).
     *
     * @param key the key that changed state
     * @param pressed @c true if the key is now pressed, @c false if is released
     */
    void keyPressed(Qt::Key key, bool pressed);

    /**
     * Emitted whenever the latched state of a key changes.
     *
     * @param key the key that changed state
     * @param latched @c true if the key is now latched, @c false if it isn't
     */
    void keyLatched(Qt::Key key, bool latched);

    /**
     * Emitted whenever the locked state of a key changes.
     *
     * @param key the key that changed state
     * @param locked @c true if the key is now locked, @c false if it isn't
     */
    void keyLocked(Qt::Key key, bool locked);

    /**
     * Emitted whenever the pressed state of a mouse button changes (mouse
     * button press or release).
     *
     * @param button the mouse button that changed state
     * @param pressed @c true if the mouse button is now pressed, @c false if
     * it is released
     */
    void buttonPressed(Qt::MouseButton button, bool pressed);

    /**
     * Emitted whenever a new modifier is found due to the keyboard mapping changing.
     *
     * @param key the key that was discovered
     */
    void keyAdded(Qt::Key key);

    /**
     * Emitted whenever a previously known modifier no longer exists due to the
     * keyboard mapping changing.
     *
     * @param key the key that vanished
     */
    void keyRemoved(Qt::Key key);

private:
    Q_DISABLE_COPY(KModifierKeyStatus)
    QExplicitlySharedDataPointer<KModifierKeyStatusProvider> const p; // krazy:exclude=dpointer
};

#endif
