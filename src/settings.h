/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SETTINGS_H
#define SETTINGS_H

#include "settingsbase.h"

class Settings : public SettingsBase
{
Q_OBJECT
public:
    static QString findNick();
};

#endif // SETTINGS_H
