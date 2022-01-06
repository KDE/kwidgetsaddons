/*
    SPDX-FileCopyrightText: 2013 Albert Vaca <albertvaka@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "lineediturldropeventfilter.h"

#include <QDropEvent>
#include <QEvent>
#include <QLineEdit>
#include <qmimedata.h>

static const char s_kdeUriListMime[] = "application/x-kde4-urilist"; // keep this name "kde4" for compat.

// KF6 TODO: add K-prefix to class name
LineEditUrlDropEventFilter::LineEditUrlDropEventFilter(QObject *parent)
    : QObject(parent)
{
}

LineEditUrlDropEventFilter::~LineEditUrlDropEventFilter()
{
}

bool LineEditUrlDropEventFilter::eventFilter(QObject *obj, QEvent *ev)
{
    // Handle only drop events
    if (ev->type() != QEvent::Drop) {
        return false;
    }
    QDropEvent *dropEv = static_cast<QDropEvent *>(ev);

    // Handle only url drops, we check the MIME type for the standard or kde's urllist
    // It would be interesting to handle urls that don't have any MIME type set (like a drag and drop from kate)
    const QMimeData *data = dropEv->mimeData();
    if (!data->hasUrls() && !data->hasFormat(QLatin1String(s_kdeUriListMime))) {
        return false;
    }

    // Our object should be a QLineEdit
    QLineEdit *line = qobject_cast<QLineEdit *>(obj);
    if (!line) {
        return false;
    }

    QString content = data->text();
    line->setText(content);
    line->setCursorPosition(content.length());

    ev->accept();
    return true;
}
