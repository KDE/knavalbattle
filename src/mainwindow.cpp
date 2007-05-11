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
#include <kstandardaction.h>
#include <kstandardgameaction.h>
#include <kactioncollection.h>
#include <kaction.h>
#include <klocale.h>

#include "playfield.h"

MainWindow::MainWindow()
{
    m_main = new PlayField(this);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_main);
    
    setCentralWidget(m_main);

    setupActions();
    resize(900, 440);
}

void MainWindow::setupActions()
{
    KAction* temp;
     
    temp = new KAction(i18n("&New Game"), this);
    temp->setShortcut(KShortcut("Ctrl+N"));
    actionCollection()->addAction("game_singleplayer", temp);
    connect(temp, SIGNAL(triggered()), m_main, SLOT(newGame()));
    
    temp = new KAction(i18n("New &Simulation"), this);
    actionCollection()->addAction("game_twomachines", temp);
    connect(temp, SIGNAL(triggered()), m_main, SLOT(newSimulation()));
    
    temp = new KAction(i18n("Start Ser&ver"), this);
    actionCollection()->addAction("game_server", temp);
    connect(temp, SIGNAL(triggered()), m_main, SLOT(newServer()));
    
    temp = new KAction(i18n("&Connect"), this);
    actionCollection()->addAction("game_client", temp);
    connect(temp, SIGNAL(triggered()), m_main, SLOT(newClient()));
    
    temp = KStandardGameAction::highscores(m_main, SLOT(highscores()), this);
    actionCollection()->addAction(temp->objectName(), temp);
    
    temp = KStandardGameAction::quit(this, SLOT(close()), this);
    actionCollection()->addAction(temp->objectName(), temp);
    
    KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
    
    setupGUI();
}

void MainWindow::optionsPreferences()
{
    kDebug() << "preferences" << endl;
}



#include "mainwindow.moc"

