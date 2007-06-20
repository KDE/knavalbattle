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
