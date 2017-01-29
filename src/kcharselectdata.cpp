/* This file is part of the KDE libraries

   Copyright (C) 2007 Daniel Laidig <d.laidig@gmx.de>
   Copyright (c) 2016 DaeHyun Sung  <sungdh86@gmail.com>

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

#include "kcharselectdata_p.h"

#include <QCoreApplication>
#include <QRegularExpression>
#include <QStringList>
#include <QFile>
#include <qendian.h>
#include <QFutureInterface>
#include <QRunnable>
#include <QThreadPool>

#include <string.h>
#include <qstandardpaths.h>

/* constants for hangul (de)composition, see UAX #15 */
#define SBase 0xAC00
#define LBase 0x1100
#define VBase 0x1161
#define TBase 0x11A7
#define LCount 19
#define VCount 21
#define TCount 28
#define NCount (VCount * TCount)
#define SCount (LCount * NCount)

class RunIndexCreation : public QFutureInterface<Index>, public QRunnable
{
public:
    RunIndexCreation(KCharSelectData *data, const QByteArray &dataFile)
        : m_data(data), m_dataFile(dataFile)
    {
    }

    QFuture<Index> start()
    {
        setRunnable(this);
        reportStarted();
        QFuture<Index> f = this->future();
        QThreadPool::globalInstance()->start(this);
        return f;
    }

    void run() Q_DECL_OVERRIDE
    {
        Index index = m_data->createIndex(m_dataFile);
        reportResult(index);
        reportFinished();
    }

private:
    KCharSelectData *m_data;
    QByteArray m_dataFile;
};

static const char JAMO_L_TABLE[][4] = {
    "G", "GG", "N", "D", "DD", "R", "M", "B", "BB",
    "S", "SS", "", "J", "JJ", "C", "K", "T", "P", "H"
};

static const char JAMO_V_TABLE[][4] = {
    "A", "AE", "YA", "YAE", "EO", "E", "YEO", "YE", "O",
    "WA", "WAE", "OE", "YO", "U", "WEO", "WE", "WI",
    "YU", "EU", "YI", "I"
};

static const char JAMO_T_TABLE[][4] = {
    "", "G", "GG", "GS", "N", "NJ", "NH", "D", "L", "LG", "LM",
    "LB", "LS", "LT", "LP", "LH", "M", "B", "BS",
    "S", "SS", "NG", "J", "C", "K", "T", "P", "H"
};

bool KCharSelectData::openDataFile()
{
    if (!dataFile.isEmpty()) {
        return true;
    } else {
        QFile file(QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kf5/kcharselect/kcharselect-data")));
        if (!file.open(QIODevice::ReadOnly)) {
            return false;
        }
        dataFile = file.readAll();
        file.close();
        if (dataFile.size() < 40) {
            dataFile.clear();
            return false;
        }
        const uchar *data = reinterpret_cast<const uchar *>(dataFile.constData());
        const quint32 offsetBegin = qFromLittleEndian<quint32>(data + 20);
        const quint32 offsetEnd = qFromLittleEndian<quint32>(data + 24);
        uint blocks = (offsetEnd - offsetBegin) / 4;
        if (blocks <= 167) { // maximum possible number of blocks in BMP
            // no remapping
            remapType = -1;
        } else if (blocks == 174) {
            // remapping introduced in 5.25
            remapType = 0;
        } else {
            // unknown remapping, abort
            dataFile.clear();
            return false;
        }
        futureIndex = (new RunIndexCreation(this, dataFile))->start();
        return true;
    }
}

// Temporary remapping code points <-> 16 bit database codes
// See kcharselect-generate-datafile.py for details

quint16 KCharSelectData::mapCodePointToDataBase(uint code) const
{
    if (remapType == 0) {
        if (code >= 0xE000 && code <= 0xEFFF) {
            return 0xFFFF;
        }
        if (code >= 0xF000 && code <= 0xFFFF) {
            return code - 0x1000;
        }
        if (code >= 0x1F000 && code <= 0x1FFFF) {
            return code - 0x10000;
        }
    }
    if (code >= 0x10000) {
        return 0xFFFF;
    }
    return code;
}

uint KCharSelectData::mapDataBaseToCodePoint(quint16 code) const
{
    if (remapType == 0) {
        if (code >= 0xE000 && code <= 0xEFFF) {
            return code + 0x1000;
        }
        if (code >= 0xF000 && code <= 0xFFFF) {
            return code + 0x10000;
        }
    }
    return code;
}

quint32 KCharSelectData::getDetailIndex(uint c) const
{
    const uchar *data = reinterpret_cast<const uchar *>(dataFile.constData());
    // Convert from little-endian, so that this code works on PPC too.
    // http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=482286
    const quint32 offsetBegin = qFromLittleEndian<quint32>(data + 12);
    const quint32 offsetEnd = qFromLittleEndian<quint32>(data + 16);

    int min = 0;
    int mid;
    int max = ((offsetEnd - offsetBegin) / 27) - 1;

    quint16 unicode = mapCodePointToDataBase(c);
    if (unicode == 0xFFFF) {
        return 0;
    }

    static quint16 most_recent_searched;
    static quint32 most_recent_result;

    if (unicode == most_recent_searched) {
        return most_recent_result;
    }

    most_recent_searched = unicode;

    while (max >= min) {
        mid = (min + max) / 2;
        const quint16 midUnicode = qFromLittleEndian<quint16>(data + offsetBegin + mid * 27);
        if (unicode > midUnicode) {
            min = mid + 1;
        } else if (unicode < midUnicode) {
            max = mid - 1;
        } else {
            most_recent_result = offsetBegin + mid * 27;

            return most_recent_result;
        }
    }

    most_recent_result = 0;
    return 0;
}

QString KCharSelectData::formatCode(uint code, int length, const QString &prefix, int base)
{
    QString s = QString::number(code, base).toUpper();
    while (s.size() < length) {
        s.prepend(QLatin1Char('0'));
    }
    s.prepend(prefix);
    return s;
}

QVector<uint> KCharSelectData::blockContents(int block)
{
    if (!openDataFile()) {
        return QVector<uint>();
    }

    const uchar *data = reinterpret_cast<const uchar *>(dataFile.constData());
    const quint32 offsetBegin = qFromLittleEndian<quint32>(data + 20);
    const quint32 offsetEnd = qFromLittleEndian<quint32>(data + 24);

    int max = ((offsetEnd - offsetBegin) / 4) - 1;

    QVector<uint> res;

    if (block > max) {
        return res;
    }

    quint16 unicodeBegin = qFromLittleEndian<quint16>(data + offsetBegin + block * 4);
    quint16 unicodeEnd = qFromLittleEndian<quint16>(data + offsetBegin + block * 4 + 2);

    while (unicodeBegin < unicodeEnd) {
        res.append(mapDataBaseToCodePoint(unicodeBegin));
        unicodeBegin++;
    }
    res.append(mapDataBaseToCodePoint(unicodeBegin)); // Be carefull when unicodeEnd==0xffff

    return res;
}

QVector<int> KCharSelectData::sectionContents(int section)
{
    if (!openDataFile()) {
        return QVector<int>();
    }

    const uchar *data = reinterpret_cast<const uchar *>(dataFile.constData());
    const quint32 offsetBegin = qFromLittleEndian<quint32>(data + 28);
    const quint32 offsetEnd = qFromLittleEndian<quint32>(data + 32);

    int max = ((offsetEnd - offsetBegin) / 4) - 1;

    QVector<int> res;

    if (section > max) {
        return res;
    }

    for (int i = 0; i <= max; i++) {
        const quint16 currSection = qFromLittleEndian<quint16>(data + offsetBegin + i * 4);
        if (currSection == section) {
            res.append(qFromLittleEndian<quint16>(data + offsetBegin + i * 4 + 2));
        }
    }

    return res;
}

QStringList KCharSelectData::sectionList()
{
    if (!openDataFile()) {
        return QStringList();
    }

    const uchar *udata = reinterpret_cast<const uchar *>(dataFile.constData());
    const quint32 stringBegin = qFromLittleEndian<quint32>(udata + 24);
    const quint32 stringEnd = qFromLittleEndian<quint32>(udata + 28);

    const char *data = dataFile.constData();
    QStringList list;
    quint32 i = stringBegin;
    while (i < stringEnd) {
        list.append(QCoreApplication::translate("KCharSelectData", data + i, "KCharSelect section name"));
        i += strlen(data + i) + 1;
    }

    return list;
}

QString KCharSelectData::block(uint c)
{
    return blockName(blockIndex(c));
}

QString KCharSelectData::section(uint c)
{
    return sectionName(sectionIndex(blockIndex(c)));
}

QString KCharSelectData::name(uint c)
{
    if (!openDataFile()) {
        return QString();
    }

    if ((c & 0xFFFE) == 0xFFFE || (c >= 0xFDD0 && c <= 0xFDEF)) {
        return QCoreApplication::translate("KCharSelectData", "<noncharacter>");
    } else if ((c >= 0x3400 && c <= 0x4DBF)
            || (c >= 0x4E00 && c <= 0x9FFF)
            || (c >= 0x20000 && c <= 0x2F7FF)) {
        return QStringLiteral("CJK UNIFIED IDEOGRAPH-") + formatCode(c, 4, QString());
    } else if (c >= 0xAC00 && c <= 0xD7AF) {
        /* compute hangul syllable name as per UAX #15 */
        int SIndex = c - SBase;
        int LIndex, VIndex, TIndex;

        if (SIndex < 0 || SIndex >= SCount) {
            return QString();
        }

        LIndex = SIndex / NCount;
        VIndex = (SIndex % NCount) / TCount;
        TIndex = SIndex % TCount;

        return QLatin1String("HANGUL SYLLABLE ") + QLatin1String(JAMO_L_TABLE[LIndex])
               + QLatin1String(JAMO_V_TABLE[VIndex]) + QLatin1String(JAMO_T_TABLE[TIndex]);
    } else if (c >= 0xD800 && c <= 0xDB7F) {
        return QCoreApplication::translate("KCharSelectData", "<Non Private Use High Surrogate>");
    } else if (c >= 0xDB80 && c <= 0xDBFF) {
        return QCoreApplication::translate("KCharSelectData", "<Private Use High Surrogate>");
    } else if (c >= 0xDC00 && c <= 0xDFFF) {
        return QCoreApplication::translate("KCharSelectData", "<Low Surrogate>");
    } else if ((c >= 0xE000 && c <= 0xF8FF) || c >= 0xF0000) {
        return QCoreApplication::translate("KCharSelectData", "<Private Use>");
    } else if ((c >= 0xF900 && c <= 0xFAFF) || (c >= 0x2F800 && c <= 0x2FFFF)) {
        return QStringLiteral("CJK COMPATIBILITY IDEOGRAPH-") + formatCode(c, 4, QString());
    }
    quint16 unicode = mapCodePointToDataBase(c);
    if (unicode == 0xFFFF) {
        return QStringLiteral("NON-BMP-CHARACTER-") + formatCode(c, 4, QString());
    } else {
        const uchar *data = reinterpret_cast<const uchar *>(dataFile.constData());
        const quint32 offsetBegin = qFromLittleEndian<quint32>(data + 4);
        const quint32 offsetEnd = qFromLittleEndian<quint32>(data + 8);

        int min = 0;
        int mid;
        int max = ((offsetEnd - offsetBegin) / 6) - 1;
        QString s;

        while (max >= min) {
            mid = (min + max) / 2;
            const quint16 midUnicode = qFromLittleEndian<quint16>(data + offsetBegin + mid * 6);
            if (unicode > midUnicode) {
                min = mid + 1;
            } else if (unicode < midUnicode) {
                max = mid - 1;
            } else {
                quint32 offset = qFromLittleEndian<quint32>(data + offsetBegin + mid * 6 + 2);
                s = QString::fromUtf8(dataFile.constData() + offset + 1);
                break;
            }
        }

        if (s.isNull()) {
            return QCoreApplication::translate("KCharSelectData", "<not assigned>");
        } else {
            return s;
        }
    }
}

int KCharSelectData::blockIndex(uint c)
{
    if (!openDataFile()) {
        return 0;
    }

    const uchar *data = reinterpret_cast<const uchar *>(dataFile.constData());
    const quint32 offsetBegin = qFromLittleEndian<quint32>(data + 20);
    const quint32 offsetEnd = qFromLittleEndian<quint32>(data + 24);
    const quint16 unicode = mapCodePointToDataBase(c);
    if (unicode == 0xFFFF) {
        return 0;
    }

    int max = ((offsetEnd - offsetBegin) / 4) - 1;

    int i = 0;

    while (unicode > qFromLittleEndian<quint16>(data + offsetBegin + i * 4 + 2) && i < max) {
        i++;
    }

    return i;
}

int KCharSelectData::sectionIndex(int block)
{
    if (!openDataFile()) {
        return 0;
    }

    const uchar *data = reinterpret_cast<const uchar *>(dataFile.constData());
    const quint32 offsetBegin = qFromLittleEndian<quint32>(data + 28);
    const quint32 offsetEnd = qFromLittleEndian<quint32>(data + 32);

    int max = ((offsetEnd - offsetBegin) / 4) - 1;

    for (int i = 0; i <= max; i++) {
        if (qFromLittleEndian<quint16>(data + offsetBegin + i * 4 + 2) == block) {
            return qFromLittleEndian<quint16>(data + offsetBegin + i * 4);
        }
    }

    return 0;
}

QString KCharSelectData::blockName(int index)
{
    if (!openDataFile()) {
        return QString();
    }

    const uchar *udata = reinterpret_cast<const uchar *>(dataFile.constData());
    const quint32 stringBegin = qFromLittleEndian<quint32>(udata + 16);
    const quint32 stringEnd = qFromLittleEndian<quint32>(udata + 20);

    quint32 i = stringBegin;
    int currIndex = 0;

    const char *data = dataFile.constData();
    while (i < stringEnd && currIndex < index) {
        i += strlen(data + i) + 1;
        currIndex++;
    }

    return QCoreApplication::translate("KCharSelectData", data + i, "KCharselect unicode block name");
}

QString KCharSelectData::sectionName(int index)
{
    if (!openDataFile()) {
        return QString();
    }

    const uchar *udata = reinterpret_cast<const uchar *>(dataFile.constData());
    const quint32 stringBegin = qFromLittleEndian<quint32>(udata + 24);
    const quint32 stringEnd = qFromLittleEndian<quint32>(udata + 28);

    quint32 i = stringBegin;
    int currIndex = 0;

    const char *data = dataFile.constData();
    while (i < stringEnd && currIndex < index) {
        i += strlen(data + i) + 1;
        currIndex++;
    }

    return QCoreApplication::translate("KCharSelectData", data + i, "KCharselect unicode section name");
}

QStringList KCharSelectData::aliases(uint c)
{
    if (!openDataFile()) {
        return QStringList();
    }
    const uchar *udata = reinterpret_cast<const uchar *>(dataFile.constData());
    const int detailIndex = getDetailIndex(c);
    if (detailIndex == 0) {
        return QStringList();
    }

    const quint8 count = * (quint8 *)(udata + detailIndex + 6);
    quint32 offset = qFromLittleEndian<quint32>(udata + detailIndex + 2);

    QStringList aliases;

    const char *data = dataFile.constData();
    for (int i = 0;  i < count;  i++) {
        aliases.append(QString::fromUtf8(data + offset));
        offset += strlen(data + offset) + 1;
    }
    return aliases;
}

QStringList KCharSelectData::notes(uint c)
{
    if (!openDataFile()) {
        return QStringList();
    }
    const int detailIndex = getDetailIndex(c);
    if (detailIndex == 0) {
        return QStringList();
    }

    const uchar *udata = reinterpret_cast<const uchar *>(dataFile.constData());
    const quint8 count = * (quint8 *)(udata + detailIndex + 11);
    quint32 offset = qFromLittleEndian<quint32>(udata + detailIndex + 7);

    QStringList notes;

    const char *data = dataFile.constData();
    for (int i = 0;  i < count;  i++) {
        notes.append(QString::fromUtf8(data + offset));
        offset += strlen(data + offset) + 1;
    }

    return notes;
}

QVector<uint> KCharSelectData::seeAlso(uint c)
{
    if (!openDataFile()) {
        return QVector<uint>();
    }
    const int detailIndex = getDetailIndex(c);
    if (detailIndex == 0) {
        return QVector<uint>();
    }

    const uchar *udata = reinterpret_cast<const uchar *>(dataFile.constData());
    const quint8 count = * (quint8 *)(udata + detailIndex + 26);
    quint32 offset = qFromLittleEndian<quint32>(udata + detailIndex + 22);

    QVector<uint> seeAlso;

    for (int i = 0;  i < count;  i++) {
        seeAlso.append(mapDataBaseToCodePoint(qFromLittleEndian<quint16> (udata + offset)));
        offset += 2;
    }

    return seeAlso;
}

QStringList KCharSelectData::equivalents(uint c)
{
    if (!openDataFile()) {
        return QStringList();
    }
    const int detailIndex = getDetailIndex(c);
    if (detailIndex == 0) {
        return QStringList();
    }

    const uchar *udata = reinterpret_cast<const uchar *>(dataFile.constData());
    const quint8 count = * (quint8 *)(udata + detailIndex + 21);
    quint32 offset = qFromLittleEndian<quint32>(udata + detailIndex + 17);

    QStringList equivalents;

    const char *data = dataFile.constData();
    for (int i = 0;  i < count;  i++) {
        equivalents.append(QString::fromUtf8(data + offset));
        offset += strlen(data + offset) + 1;
    }

    return equivalents;
}

QStringList KCharSelectData::approximateEquivalents(uint c)
{
    if (!openDataFile()) {
        return QStringList();
    }
    const int detailIndex = getDetailIndex(c);
    if (detailIndex == 0) {
        return QStringList();
    }

    const uchar *udata = reinterpret_cast<const uchar *>(dataFile.constData());
    const quint8 count = * (quint8 *)(udata + detailIndex + 16);
    quint32 offset = qFromLittleEndian<quint32>(udata + detailIndex + 12);

    QStringList approxEquivalents;

    const char *data = dataFile.constData();
    for (int i = 0;  i < count;  i++) {
        approxEquivalents.append(QString::fromUtf8(data + offset));
        offset += strlen(data + offset) + 1;
    }

    return approxEquivalents;
}

QStringList KCharSelectData::unihanInfo(uint c)
{
    if (!openDataFile()) {
        return QStringList();
    }

    quint16 unicode = mapCodePointToDataBase(c);
    if (unicode == 0xFFFF) {
        return QStringList();
    }

    const char *data = dataFile.constData();
    const uchar *udata = reinterpret_cast<const uchar *>(data);
    const quint32 offsetBegin = qFromLittleEndian<quint32>(udata + 36);
    const quint32 offsetEnd = dataFile.size();

    int min = 0;
    int mid;
    int max = ((offsetEnd - offsetBegin) / 38) - 1;

    while (max >= min) {
        mid = (min + max) / 2;
        const quint16 midUnicode = qFromLittleEndian<quint16>(udata + offsetBegin + mid * 38);
        if (unicode > midUnicode) {
            min = mid + 1;
        } else if (unicode < midUnicode) {
            max = mid - 1;
        } else {
            QStringList res;
            for (int i = 0; i < 9; i++) {
                quint32 offset = qFromLittleEndian<quint32>(udata + offsetBegin + mid * 38 + 2 + i * 4);
                if (offset != 0) {
                    res.append(QString::fromUtf8(data + offset));
                } else {
                    res.append(QString());
                }
            }
            return res;
        }
    }

    return QStringList();
}

QChar::Category KCharSelectData::category(uint c)
{
    if (!openDataFile()) {
        return QChar::category(c);
    }

    ushort unicode = mapCodePointToDataBase(c);
    if (unicode == 0xFFFF) {
        return QChar::category(c);
    }

    const uchar *data = reinterpret_cast<const uchar *>(dataFile.constData());
    const quint32 offsetBegin = qFromLittleEndian<quint32>(data + 4);
    const quint32 offsetEnd = qFromLittleEndian<quint32>(data + 8);

    int min = 0;
    int mid;
    int max = ((offsetEnd - offsetBegin) / 6) - 1;
    QString s;

    while (max >= min) {
        mid = (min + max) / 2;
        const quint16 midUnicode = qFromLittleEndian<quint16>(data + offsetBegin + mid * 6);
        if (unicode > midUnicode) {
            min = mid + 1;
        } else if (unicode < midUnicode) {
            max = mid - 1;
        } else {
            quint32 offset = qFromLittleEndian<quint32>(data + offsetBegin + mid * 6 + 2);
            uchar categoryCode = *(data + offset);
            Q_ASSERT(categoryCode > 0);
            categoryCode--;  /* Qt5 changed QChar::Category enum to start from 0 instead of 1
                                See QtBase commit d17c76feee9eece4 */
            return QChar::Category(categoryCode);
        }
    }

    return QChar::category(c);
}

bool KCharSelectData::isPrint(uint c)
{
    QChar::Category cat = category(c);
    return !(cat == QChar::Other_Control || cat == QChar::Other_NotAssigned);
}

bool KCharSelectData::isDisplayable(uint c)
{
    // Qt internally uses U+FDD0 and U+FDD1 to mark the beginning and the end of frames.
    // They should be seen as non-printable characters, as trying to display them leads
    //  to a crash caused by a Qt "noBlockInString" assertion.
    if (c == 0xFDD0 || c == 0xFDD1) {
        return false;
    }

    return !isIgnorable(c) && isPrint(c);
}

bool KCharSelectData::isIgnorable(uint c)
{
    /*
     * According to the Unicode standard, Default Ignorable Code Points
     * should be ignored unless explicitly supported. For example, U+202E
     * RIGHT-TO-LEFT-OVERRIDE ir printable according to Qt, but displaying
     * it gives the undesired effect of all text being turned RTL. We do not
     * have a way to "explicitly" support it, so we will treat it as
     * non-printable.
     *
     * There is a list of these on
     * http://unicode.org/Public/UNIDATA/DerivedCoreProperties.txt under the
     * property Default_Ignorable_Code_Point.
     */

    //NOTE: not very nice to hardcode these here; is it worth it to modify
    //      the binary data file to hold them?
    return c == 0x00AD || c == 0x034F || c == 0x115F || c == 0x1160 ||
           c == 0x17B4 || c == 0x17B5 || (c >= 0x180B && c <= 0x180D) ||
           (c >= 0x200B && c <= 0x200F) || (c >= 0x202A && c <= 0x202E) ||
           (c >= 0x2060 && c <= 0x206F) || c == 0x3164 ||
           (c >= 0xFE00 && c <= 0xFE0F) || c == 0xFEFF || c == 0xFFA0 ||
           (c >= 0xFFF0 && c <= 0xFFF8);
}

bool KCharSelectData::isCombining(uint c)
{
    return section(c) == QCoreApplication::translate("KCharSelectData", "Combining Diacritics", "KCharSelect section name");
    //FIXME: this is an imperfect test. There are many combining characters
    //       that are outside of this section. See Grapheme_Extend in
    //       http://www.unicode.org/Public/UNIDATA/DerivedCoreProperties.txt
}

QString KCharSelectData::display(uint c, const QFont &font)
{
    if (!isDisplayable(c)) {
        return QStringLiteral("<b>") + QCoreApplication::translate("KCharSelectData", "Non-printable") + QStringLiteral("</b>");
    } else {
        QString s = QStringLiteral("<font size=\"+4\" face=\"") + font.family() + QStringLiteral("\">");
        if (isCombining(c)) {
            s += displayCombining(c);
        } else {
            s += QStringLiteral("&#") + QString::number(c) + QLatin1Char(';');
        }
        s += QStringLiteral("</font>");
        return s;
    }
}

QString KCharSelectData::displayCombining(uint c)
{
    /*
     * The purpose of this is to make it easier to see how a combining
     * character affects the text around it.
     * The initial plan was to use U+25CC DOTTED CIRCLE for this purpose,
     * as seen in pdfs from Unicode, but there seem to be a lot of alignment
     * problems with that.
     *
     * Eventually, it would be nice to determine whether the character
     * combines to the left or to the right, etc.
     */
    QString s = QStringLiteral("&nbsp;&#") + QString::number(c) + QStringLiteral(";&nbsp;") +
                QStringLiteral(" (ab&#") + QString::number(c) + QStringLiteral(";c)");
    return s;
}

QString KCharSelectData::categoryText(QChar::Category category)
{
    switch (category) {
    case QChar::Other_Control: return QCoreApplication::translate("KCharSelectData", "Other, Control");
    case QChar::Other_Format: return QCoreApplication::translate("KCharSelectData", "Other, Format");
    case QChar::Other_NotAssigned: return QCoreApplication::translate("KCharSelectData", "Other, Not Assigned");
    case QChar::Other_PrivateUse: return QCoreApplication::translate("KCharSelectData", "Other, Private Use");
    case QChar::Other_Surrogate: return QCoreApplication::translate("KCharSelectData", "Other, Surrogate");
    case QChar::Letter_Lowercase: return QCoreApplication::translate("KCharSelectData", "Letter, Lowercase");
    case QChar::Letter_Modifier: return QCoreApplication::translate("KCharSelectData", "Letter, Modifier");
    case QChar::Letter_Other: return QCoreApplication::translate("KCharSelectData", "Letter, Other");
    case QChar::Letter_Titlecase: return QCoreApplication::translate("KCharSelectData", "Letter, Titlecase");
    case QChar::Letter_Uppercase: return QCoreApplication::translate("KCharSelectData", "Letter, Uppercase");
    case QChar::Mark_SpacingCombining: return QCoreApplication::translate("KCharSelectData", "Mark, Spacing Combining");
    case QChar::Mark_Enclosing: return QCoreApplication::translate("KCharSelectData", "Mark, Enclosing");
    case QChar::Mark_NonSpacing: return QCoreApplication::translate("KCharSelectData", "Mark, Non-Spacing");
    case QChar::Number_DecimalDigit: return QCoreApplication::translate("KCharSelectData", "Number, Decimal Digit");
    case QChar::Number_Letter: return QCoreApplication::translate("KCharSelectData", "Number, Letter");
    case QChar::Number_Other: return QCoreApplication::translate("KCharSelectData", "Number, Other");
    case QChar::Punctuation_Connector: return QCoreApplication::translate("KCharSelectData", "Punctuation, Connector");
    case QChar::Punctuation_Dash: return QCoreApplication::translate("KCharSelectData", "Punctuation, Dash");
    case QChar::Punctuation_Close: return QCoreApplication::translate("KCharSelectData", "Punctuation, Close");
    case QChar::Punctuation_FinalQuote: return QCoreApplication::translate("KCharSelectData", "Punctuation, Final Quote");
    case QChar::Punctuation_InitialQuote: return QCoreApplication::translate("KCharSelectData", "Punctuation, Initial Quote");
    case QChar::Punctuation_Other: return QCoreApplication::translate("KCharSelectData", "Punctuation, Other");
    case QChar::Punctuation_Open: return QCoreApplication::translate("KCharSelectData", "Punctuation, Open");
    case QChar::Symbol_Currency: return QCoreApplication::translate("KCharSelectData", "Symbol, Currency");
    case QChar::Symbol_Modifier: return QCoreApplication::translate("KCharSelectData", "Symbol, Modifier");
    case QChar::Symbol_Math: return QCoreApplication::translate("KCharSelectData", "Symbol, Math");
    case QChar::Symbol_Other: return QCoreApplication::translate("KCharSelectData", "Symbol, Other");
    case QChar::Separator_Line: return QCoreApplication::translate("KCharSelectData", "Separator, Line");
    case QChar::Separator_Paragraph: return QCoreApplication::translate("KCharSelectData", "Separator, Paragraph");
    case QChar::Separator_Space: return QCoreApplication::translate("KCharSelectData", "Separator, Space");
    default: return QCoreApplication::translate("KCharSelectData", "Unknown");
    }
}

QVector<uint> KCharSelectData::find(const QString &needle)
{
    QSet<uint> result;

    QVector<uint> returnRes;
    QString simplified = needle.simplified();
    QStringList searchStrings;

    QRegularExpression octalExp(QStringLiteral("^\\\\[0-7][0-7\\\\]*$"));
    QRegularExpressionMatch match = octalExp.match(simplified);
    if (match.hasMatch()) {
        // search for C octal escaped UTF-8
        QByteArray utf8;
        int byte = -1;
        for (int i = 0; i <= simplified.length(); ++i) {
            int c = simplified.at(i).unicode();
            if (c >= '0' && c <= '7') {
                byte = 8 * byte + c - '0';
            } else if (byte == -1) {
                byte = 0;
            } else if (byte >= 0x00 && byte <= 0xFF) {
                utf8.append((char) byte);
                byte = 0;
            }
        }
        simplified = QString::fromUtf8(utf8);
    }

    if (simplified.length() <= 2) {
        QVector<uint> ucs4 = simplified.toUcs4();
        if (ucs4.size() == 1) {
            // search for hex representation of the character
            searchStrings = QStringList(formatCode(ucs4.at(0)));
        }
    } else {
        searchStrings = splitString(simplified);
    }

    if (searchStrings.count() == 0) {
        return returnRes;
    }

    QRegularExpression hexExp(QStringLiteral("^(|u\\+|U\\+|0x|0X)([A-Fa-f0-9]{4,5})$"));
    foreach (const QString &s, searchStrings) {
        QRegularExpressionMatch match = hexExp.match(s);
        if (match.hasMatch()) {
            returnRes.append(match.captured(2).toInt(nullptr, 16));
            // search for "1234" instead of "0x1234"
            if (s.length() == 6 || s.length() == 7) {
                searchStrings[searchStrings.indexOf(s)] = match.captured(2);
            }
        }
        // try to parse string as decimal number
        bool ok;
        int unicode = s.toInt(&ok);
        if (ok && unicode >= 0 && unicode <= QChar::LastValidCodePoint) {
            returnRes.append(unicode);
        }
    }

    bool firstSubString = true;
    foreach (const QString &s, searchStrings) {
        QSet<uint> partResult = getMatchingChars(s.toLower());
        if (firstSubString) {
            result = partResult;
            firstSubString = false;
        } else {
            result = result.intersect(partResult);
        }
    }

    // remove results found by matching the code point to prevent duplicate results
    // while letting these characters stay at the beginning
    foreach (uint c, returnRes) {
        result.remove(c);
    }

    QVector<uint> sortedResult;
    sortedResult.reserve(result.count());
    QSet<uint>::const_iterator it = result.begin();
    const QSet<uint>::const_iterator end = result.end();
    for ( ; it != end ; ++it ) {
        sortedResult.append(*it);
    }
    qSort(sortedResult);

    returnRes += sortedResult;
    return returnRes;
}

QSet<uint> KCharSelectData::getMatchingChars(const QString &s)
{
    if (dataFile.isEmpty()) {
        return QSet<uint>();
    }
    futureIndex.waitForFinished();
    const Index index = futureIndex;
    Index::const_iterator pos = index.lowerBound(s);
    QSet<uint> result;

    while (pos != index.constEnd() && pos.key().startsWith(s)) {
        foreach (quint16 c, pos.value()) {
            result.insert(mapDataBaseToCodePoint(c));
        }
        ++pos;
    }

    return result;
}

QStringList KCharSelectData::splitString(const QString &s)
{
    QStringList result;
    int start = 0;
    int end = 0;
    int length = s.length();
    while (end < length) {
        while (end < length && (s[end].isLetterOrNumber() || s[end] == QLatin1Char('+'))) {
            end++;
        }
        if (start != end) {
            result.append(s.mid(start, end - start));
        }
        start = end;
        while (end < length && !(s[end].isLetterOrNumber() || s[end] == QLatin1Char('+'))) {
            end++;
            start++;
        }
    }
    return result;
}

void KCharSelectData::appendToIndex(Index *index, quint16 unicode, const QString &s)
{
    const QStringList strings = splitString(s);
    foreach (const QString &s, strings) {
        (*index)[s.toLower()].append(unicode);
    }
}

Index KCharSelectData::createIndex(const QByteArray &dataFile)
{
    Index i;

    // character names
    const uchar *udata = reinterpret_cast<const uchar *>(dataFile.constData());
    const char *data = dataFile.constData();
    const quint32 nameOffsetBegin = qFromLittleEndian<quint32>(udata + 4);
    const quint32 nameOffsetEnd = qFromLittleEndian<quint32>(udata + 8);

    int max = ((nameOffsetEnd - nameOffsetBegin) / 6) - 1;

    for (int pos = 0; pos <= max; pos++) {
        const quint16 unicode = qFromLittleEndian<quint16>(udata + nameOffsetBegin + pos * 6);
        quint32 offset = qFromLittleEndian<quint32>(udata + nameOffsetBegin + pos * 6 + 2);
        appendToIndex(&i, unicode, QString::fromUtf8(data + offset + 1));
    }

    // details
    const quint32 detailsOffsetBegin = qFromLittleEndian<quint32>(udata + 12);
    const quint32 detailsOffsetEnd = qFromLittleEndian<quint32>(udata + 16);

    max = ((detailsOffsetEnd - detailsOffsetBegin) / 27) - 1;

    for (int pos = 0; pos <= max; pos++) {
        const quint16 unicode = qFromLittleEndian<quint16>(udata + detailsOffsetBegin + pos * 27);

        // aliases
        const quint8 aliasCount = * (quint8 *)(udata + detailsOffsetBegin + pos * 27 + 6);
        quint32 aliasOffset = qFromLittleEndian<quint32>(udata + detailsOffsetBegin + pos * 27 + 2);

        for (int j = 0;  j < aliasCount;  j++) {
            appendToIndex(&i, unicode, QString::fromUtf8(data + aliasOffset));
            aliasOffset += strlen(data + aliasOffset) + 1;
        }

        // notes
        const quint8 notesCount = * (quint8 *)(udata + detailsOffsetBegin + pos * 27 + 11);
        quint32 notesOffset = qFromLittleEndian<quint32>(udata + detailsOffsetBegin + pos * 27 + 7);

        for (int j = 0;  j < notesCount;  j++) {
            appendToIndex(&i, unicode, QString::fromUtf8(data + notesOffset));
            notesOffset += strlen(data + notesOffset) + 1;
        }

        // approximate equivalents
        const quint8 apprCount = * (quint8 *)(udata + detailsOffsetBegin + pos * 27 + 16);
        quint32 apprOffset = qFromLittleEndian<quint32>(udata + detailsOffsetBegin + pos * 27 + 12);

        for (int j = 0;  j < apprCount;  j++) {
            appendToIndex(&i, unicode, QString::fromUtf8(data + apprOffset));
            apprOffset += strlen(data + apprOffset) + 1;
        }

        // equivalents
        const quint8 equivCount = * (quint8 *)(udata + detailsOffsetBegin + pos * 27 + 21);
        quint32 equivOffset = qFromLittleEndian<quint32>(udata + detailsOffsetBegin + pos * 27 + 17);

        for (int j = 0;  j < equivCount;  j++) {
            appendToIndex(&i, unicode, QString::fromUtf8(data + equivOffset));
            equivOffset += strlen(data + equivOffset) + 1;
        }

        // see also - convert to string (hex)
        const quint8 seeAlsoCount = * (quint8 *)(udata + detailsOffsetBegin + pos * 27 + 26);
        quint32 seeAlsoOffset = qFromLittleEndian<quint32>(udata + detailsOffsetBegin + pos * 27 + 22);

        for (int j = 0;  j < seeAlsoCount;  j++) {
            quint16 seeAlso = qFromLittleEndian<quint16> (udata + seeAlsoOffset);
            appendToIndex(&i, unicode, formatCode(seeAlso, 4, QString()));
            equivOffset += strlen(data + equivOffset) + 1;
        }
    }

    // unihan data
    // temporary disabled due to the huge amount of data
//     const quint32 unihanOffsetBegin = qFromLittleEndian<quint32>(udata+36);
//     const quint32 unihanOffsetEnd = dataFile.size();
//     max = ((unihanOffsetEnd - unihanOffsetBegin) / 30) - 1;
//
//     for (int pos = 0; pos <= max; pos++) {
//         const quint16 unicode = qFromLittleEndian<quint16>(udata + unihanOffsetBegin + pos*30);
//         for(int j = 0; j < 7; j++) {
//             quint32 offset = qFromLittleEndian<quint32>(udata + unihanOffsetBegin + pos*30 + 2 + j*4);
//             if(offset != 0) {
//                 appendToIndex(&i, unicode, QString::fromUtf8(data + offset));
//             }
//         }
//     }

    return i;
}
