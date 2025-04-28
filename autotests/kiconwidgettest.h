/*
 * SPDX-FileCopyrightText: 2025 Kai Uwe Broulik <kde@broulik.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KICONWIDGETTEST_H
#define KICONWIDGETTEST_H

#include <QObject>

class KIconWidgetTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testDefaults();
    void testSize();
    void testRendering();
    void testMode_data();
    void testMode();
};

#endif // KICONWIDGETTEST_H
