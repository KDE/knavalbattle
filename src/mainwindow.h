/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
  Copyright (c) 2007 Riccardo Iaconelli <ruphy@fsfe.org>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>
#include "ui_preferences.h"

class PlayField;

namespace KGGZMod
{
    class Module;
    class Event;
}

class KGGZRaw;

class MainWindow : public KXmlGuiWindow
{
Q_OBJECT
    PlayField* m_main;
    Ui_Preferences m_pref_ui;
public:
    MainWindow();
protected:
    void setupActions();
protected slots:
    void optionsPreferences();
private slots:
    void networkErrorHandler();
    void networkData(int fd);
    void networkEvent(const KGGZMod::Event& event);
    
    void startingGame();
    void welcomeScreen();
private:
    KGGZMod::Module *m_mod;
    KGGZRaw *m_raw;
    int m_fd;
    bool m_ggzsetup;
};

#endif // MAINWINDOW_H

