/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 2007 Daniel Laidig <d.laidig@gmx.de>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCHARSELECTDATA_H
#define KCHARSELECTDATA_H

#include <QChar>
#include <QFont>
#include <QFuture>
#include <QString>
#include <QStringList>
#include <QVector>

// Internal class used by KCharSelect

typedef QMap<QString, QVector<quint16>> Index;

class KCharSelectData
{
public:
    QString formatCode(uint code, int length = 4, const QString &prefix = QStringLiteral("U+"), int base = 16);

    QVector<uint> blockContents(int block);
    QVector<int> sectionContents(int section);

    QStringList sectionList();

    QString block(uint c);
    QString section(uint c);
    QString name(uint c);

    int blockIndex(uint c);
    int sectionIndex(int block);

    QString blockName(int index);
    QString sectionName(int index);

    QStringList aliases(uint c);
    QStringList notes(uint c);
    QVector<uint> seeAlso(uint c);
    QStringList equivalents(uint c);
    QStringList approximateEquivalents(uint c);
    QVector<uint> decomposition(uint c);

    QStringList unihanInfo(uint c);

    QChar::Category category(uint c);
    bool isPrint(uint c);
    bool isDisplayable(uint c);
    bool isIgnorable(uint c);
    bool isCombining(uint c);
    QString display(uint c, const QFont &font);
    QString displayCombining(uint c);

    QString categoryText(QChar::Category category);

    QVector<uint> find(const QString &s);

private:
    bool openDataFile();
    quint32 getDetailIndex(uint c) const;
    QSet<uint> getMatchingChars(const QString &s);

    QStringList splitString(const QString &s);
    void appendToIndex(Index *index, quint16 unicode, const QString &s);
    Index createIndex(const QByteArray &dataFile);

    quint16 mapCodePointToDataBase(uint code) const;
    uint mapDataBaseToCodePoint(quint16 code) const;

    QByteArray dataFile;
    QFuture<Index> futureIndex;
    int remapType;
    friend class RunIndexCreation;
};

#endif /* #ifndef KCHARSELECTDATA_H */
