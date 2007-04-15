/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
  Copyright (c) 2007 Riccardo Iaconelli <ruphy@fsfe.org>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include <QResizeEvent>
#include <QHBoxLayout>
#include <kstandardaction.h>
#include <kactioncollection.h>
#include <kaction.h>
#include <klocale.h>

#include "mainwindow.h"
#include "playfield.h"
#include <kwelcomescreen.h>

MainWindow::MainWindow()
{
    welcomeScreen = new KWelcomeScreen(this);
    m_main = new PlayField(welcomeScreen);
//     QVBoxLayoutdddd
    setCentralWidget(welcomeScreen);
//     m_main = new PlayField(this);
//     setCentralWidget(m_main);
    welcomeScreen->addButton("Start game", KIcon("system-run"), "game_singleplayer", 0, 0);
    welcomeScreen->addButton("Simulation", KIcon("roll"), "game_twomachines", 1, 0);
    welcomeScreen->addButton("Start server", KIcon("network"), "game_server", 0, 1);
    welcomeScreen->addButton("Start client", KIcon("user-female"), "game_client", 1, 1);
    welcomeScreen->addButton("Highscores", KIcon("arrow-up-double"), "show_highscores", 0, 2);
    welcomeScreen->init();
    setupActions();
    connect(welcomeScreen, SIGNAL(buttonClicked(QString)), this, SLOT(welcomeScreenSlot(QString)));
    connect(welcomeScreen, SIGNAL(resized(QSize)), this, SLOT(resizeSlot(QSize)));
    connect(m_main, SIGNAL(gameFinished()), welcomeScreen, SLOT(showOverlay()));
    m_main->resize(900, 400);
    welcomeScreen->resize(m_main->size());
    resize(900, 440);
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
    
    temp = new KAction(i18n("&Connect"), this);
    actionCollection()->addAction("game_client", temp);
    connect(temp, SIGNAL(triggered()), m_main, SLOT(newClient()));
    
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

void MainWindow::welcomeScreenSlot(QString button)
{
    if (button == "game_singleplayer") {
        m_main->newGame();
        welcomeScreen->hideOverlay();
    } else if (button == "game_twomachines") {
        m_main->newSimulation();
        welcomeScreen->hideOverlay();
    } else if (button == "game_server") {
        m_main->newServer();
        welcomeScreen->hideOverlay();
    } else if (button == "game_client") {
        m_main->newClient();
        welcomeScreen->hideOverlay();
    } else if (button == "show_highscores") {
        m_main->highscores();
    }
}

void MainWindow::resizeSlot(QSize size)
{
    m_main->resize(size);
}

#include "mainwindow.moc"

