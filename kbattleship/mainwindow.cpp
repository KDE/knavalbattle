/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include <QResizeEvent>
#include <kstandardaction.h>
#include <kactioncollection.h>
#include <kaction.h>
#include <klocale.h>

#include "mainwindow.h"
#include "playfield.h"

MainWindow::MainWindow()
{
    m_main = new PlayField(this);
    setCentralWidget(m_main);
    setupActions();
    
    resize(900, 400);
}

void MainWindow::setupActions()
{
    KAction* temp;
     
    temp = new KAction(i18n("&New game"), this);
    actionCollection()->addAction("game_singleplayer", temp);
    connect(temp, SIGNAL(triggered()), m_main, SLOT(newGame()));
    
    temp = new KAction(i18n("New &simulation"), this);
    actionCollection()->addAction("game_twomachines", temp);
    connect(temp, SIGNAL(triggered()), m_main, SLOT(newSimulation()));
    
    temp = new KAction(i18n("Start ser&ver"), this);
    actionCollection()->addAction("game_server", temp);
    connect(temp, SIGNAL(triggered()), m_main, SLOT(newServer()));
    
    temp = new KAction(i18n("Show highscores"), this);
    actionCollection()->addAction("show_highscores", temp);
    connect(temp, SIGNAL(triggered()), m_main, SLOT(highscores()));
    
    KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
    
    setupGUI();
}

void MainWindow::optionsPreferences()
{
    kDebug() << "preferences" << endl;
}

#include "mainwindow.moc"

