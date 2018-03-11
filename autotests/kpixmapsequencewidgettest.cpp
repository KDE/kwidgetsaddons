/* This file is part of the KDE libraries
 *
 * Copyright (c) 2017 David Faure <faure@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */
#include <KPixmapSequenceWidget>

#include <QTest>

#include "kguiitem.h"

static const QString INACTIVE_TEXT = QStringLiteral("Show Foo");
static const QString ACTIVE_TEXT = QStringLiteral("Hide Foo");

class KPixmapSequenceWidgetTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase()
    {
    }

    void testConstructor()
    {
        KPixmapSequenceWidget w;
    }
};

QTEST_MAIN(KPixmapSequenceWidgetTest)

#include "kpixmapsequencewidgettest.moc"
