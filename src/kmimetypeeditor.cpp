/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 2014 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only or LicenseRef-KDE-Accepted-LGPL
*/

#include "kmimetypeeditor.h"

#include <QProcess>

static const char s_keditfiletypeExecutable[] = "keditfiletype5";

void KMimeTypeEditor::editMimeType(const QString &mimeType, QWidget *widget)
{
    QStringList args;
#ifndef Q_OS_WIN
    args << QStringLiteral("--parent") << QString::number(widget->window()->winId());
#endif
    args << mimeType;

    QProcess::startDetached(QString::fromLatin1(s_keditfiletypeExecutable), args);
}

