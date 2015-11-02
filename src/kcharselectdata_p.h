/* This file is part of the KDE libraries

   Copyright (C) 2007 Daniel Laidig <d.laidig@gmx.de>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KCHARSELECTDATA_H
#define KCHARSELECTDATA_H

#include <QtCore/QChar>
#include <QtCore/QVector>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QFuture>
#include <QFont>

// Internal class used by KCharSelect

typedef QMap<QString, QVector<quint16> > Index;

class KCharSelectData
{
public:
    QString formatCode(ushort code, int length = 4, const QString &prefix = QStringLiteral("U+"), int base = 16);

    QVector<QChar> blockContents(int block);
    QVector<int> sectionContents(int section);

    QStringList sectionList();

    QString block(QChar c);
    QString section(QChar c);
    QString name(QChar c);

    int blockIndex(QChar c);
    int sectionIndex(int block);

    QString blockName(int index);
    QString sectionName(int index);

    QStringList aliases(QChar c);
    QStringList notes(QChar c);
    QVector<QChar> seeAlso(QChar c);
    QStringList equivalents(QChar c);
    QStringList approximateEquivalents(QChar c);

    QStringList unihanInfo(QChar c);

    QChar::Category category(QChar c);
    bool isPrint(QChar c);
    bool isDisplayable(QChar c);
    bool isIgnorable(QChar c);
    bool isCombining(QChar c);
    QString display(QChar c, const QFont &font);
    QString displayCombining(QChar c);

    QString categoryText(QChar::Category category);

    QVector<QChar> find(const QString &s);

private:
    bool openDataFile();
    quint32 getDetailIndex(QChar c) const;
    QSet<QChar> getMatchingChars(const QString &s);

    QStringList splitString(const QString &s);
    void appendToIndex(Index *index, quint16 unicode, const QString &s);
    Index createIndex(const QByteArray &dataFile);

    QByteArray dataFile;
    QFuture<Index> futureIndex;
    friend class RunIndexCreation;
};

#endif  /* #ifndef KCHARSELECTDATA_H */
