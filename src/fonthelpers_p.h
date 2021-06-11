/*
    SPDX-FileCopyrightText: 2008 Chusslove Illich <caslav.ilic@gmx.net>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef FONTHELPERS_P_H
#define FONTHELPERS_P_H

// i18n-related helpers for fonts, common to KFont* widgets.

#include <QString>
#include <QStringList>

#include <map>

static bool fontFamilyCompare(const QString &a, const QString &b)
{
    return QString::localeAwareCompare(a, b) < 0;
}

using FontFamiliesMap = std::map<QString, QString, decltype(fontFamilyCompare) *>;

/**
 * @internal
 *
 * Split the compound raw font name into family and foundry.
 *
 * @param name the raw font name reported by Qt
 * @param family the storage for family name
 * @param foundry the storage for foundry name
 */
void splitFontString(const QString &name, QString *family, QString *foundry = nullptr);

/**
 * @internal
 *
 * Translate the font name for the user.
 * Primarily for generic fonts like Serif, Sans-Serif, etc.
 *
 * @param name the raw font name reported by Qt
 * @return translated font name
 */
QString translateFontName(const QString &name);

/**
 * @internal
 *
 * Compose locale-aware sorted list of translated font names,
 * with generic fonts handled in a special way.
 * The mapping of translated to raw names can be reported too if required.
 *
 * @param names raw font names as reported by Qt
 * @param trToRawNames storage for mapping of translated to raw names
 * @return sorted list of translated font names
 */
FontFamiliesMap translateFontNameList(const QStringList &names);

#endif
