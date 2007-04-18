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

#include <kxmlguiwindow.h>

class PlayField;

class MainWindow : public KXmlGuiWindow
{
Q_OBJECT
    PlayField* m_main;
public:
    MainWindow();
protected:
    void setupActions();
protected slots:
    void optionsPreferences();
};

#endif // MAINWINDOW_H

