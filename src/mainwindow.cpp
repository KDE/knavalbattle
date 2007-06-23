/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
  Copyright (c) 2007 Riccardo Iaconelli <ruphy@fsfe.org>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "mainwindow.h"

#include <QResizeEvent>
#include <QHBoxLayout>


#include <KAction>
#include <KActionCollection>
#include <KLocalizedString>
#include <KStandardAction>
#include <KStandardGameAction>
#include <KStatusBar>

#include "playfield.h"

MainWindow::MainWindow()
{
    m_main = new PlayField(this, statusBar());
    
    setCentralWidget(m_main);

    setupActions();
    resize(900, 440);
}

void MainWindow::setupActions()
{
    // Game
    KStandardGameAction::gameNew(m_main, SLOT(newGame()), actionCollection());
//     KStandardGameAction::demo(m_main, SLOT(newSimulation()), actionCollection());
    KStandardGameAction::restart(m_main, SLOT(restart()), actionCollection());
//     
    KStandardGameAction::highscores(m_main, SLOT(highscores()), actionCollection());
    KStandardGameAction::quit(this, SLOT(close()), actionCollection());

    // Move

    // Settings
    KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
     
//     temp = new KAction(i18n("Start Ser&ver"), this);
//     actionCollection()->addAction("game_server", temp);
//     connect(temp, SIGNAL(triggered()), m_main, SLOT(newServer()));
//     
//     temp = new KAction(i18n("&Connect"), this);
//     actionCollection()->addAction("game_client", temp);
//     connect(temp, SIGNAL(triggered()), m_main, SLOT(newClient()));
    
    setupGUI();
}

void MainWindow::optionsPreferences()
{
    kDebug() << "preferences" << endl;
}



#include "mainwindow.moc"

