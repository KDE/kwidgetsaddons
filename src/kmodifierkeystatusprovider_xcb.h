/*
    SPDX-FileCopyrightText: 2009 Michael Leupold <lemma@confuego.org>
    SPDX-FileCopyrightText: 2013 Martin Gräßlin <mgraesslin@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KMODIFIERKEYSTATUSPROVIDERXCB_H
#define KMODIFIERKEYSTATUSPROVIDERXCB_H

#include "kmodifierkeystatusprovider_p.h"
#include <QAbstractNativeEventFilter>
class KModifierKeyStatusProviderXcb : public KModifierKeyStatusProvider, public QAbstractNativeEventFilter
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.kwidgetsaddons.KModifierKeyStatusProvider.XCB")
public:
    KModifierKeyStatusProviderXcb();
    ~KModifierKeyStatusProviderXcb();

    bool setKeyLatched(Qt::Key key, bool latched) override;
    bool setKeyLocked(Qt::Key key, bool locked) override;

    bool nativeEventFilter(const QByteArray &eventType, void *message, long int *result) override;

    void xkbUpdateModifierMapping();
    void xkbModifierStateChanged(unsigned char mods, unsigned char latched_mods, unsigned char locked_mods);
    void xkbButtonStateChanged(unsigned short ptr_buttons);

private:
    int m_xkbEv;
    bool m_xkbAvailable;

    // maps a Qt::Key to a modifier mask
    QHash<Qt::Key, unsigned int> m_xkbModifiers;
    // maps a Qt::MouseButton to a button mask
    QHash<Qt::MouseButton, unsigned short> m_xkbButtons;
};

#endif
