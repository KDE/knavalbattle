/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
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
