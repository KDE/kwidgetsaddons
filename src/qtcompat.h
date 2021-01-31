/*
    SPDX-FileCopyrightText: 2020 Nicolas Fella <nicolas.fella@gmx.de>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
using EnterEvent = QEnterEvent;
#else
using EnterEvent = QEvent;
#endif
