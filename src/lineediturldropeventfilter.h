/*
    SPDX-FileCopyrightText: 2013 Albert Vaca <albertvaka@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef LINEEDITURLDROPEVENTFILTER_H
#define LINEEDITURLDROPEVENTFILTER_H

#include <kwidgetsaddons_export.h>

#include <QObject>

/**
 * @class LineEditUrlDropEventFilter lineediturldropeventfilter.h LineEditUrlDropEventFilter
 *
 * This class provides an event filter that can be installed on a QLineEdit
 * or a subclass of it (KLineEdit) to make it handle url drop events so
 * when a url is dropped it replaces the existing content.
 */
class KWIDGETSADDONS_EXPORT LineEditUrlDropEventFilter
    : public QObject
{
    Q_OBJECT

public:
    explicit LineEditUrlDropEventFilter(QObject *parent = nullptr);
    ~LineEditUrlDropEventFilter() override;
    bool eventFilter(QObject *obj, QEvent *ev) override;
};

#endif
