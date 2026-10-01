/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 2025 g10 Code GmbH
    SPDX-FileContributor: Ingo Klöcker <dev@ingo-kloecker.de>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "highcontrasthelper_p.h"

#include <QAccessibilityHints>
#include <QGuiApplication>
#include <QStyleHints>

static bool isHighContrastModeActive()
{
    return QGuiApplication::styleHints()->accessibility()->contrastPreference() == Qt::ContrastPreference::HighContrast;
}

static bool isDefaultColorSchemeInUse()
{
    const QVariant colorSchemePathProperty = qApp->property("KDE_COLOR_SCHEME_PATH");
    return !colorSchemePathProperty.isValid() || colorSchemePathProperty.toString().isEmpty();
}

bool isHighContrastColorSchemeInUse()
{
    return isHighContrastModeActive() && isDefaultColorSchemeInUse();
}
