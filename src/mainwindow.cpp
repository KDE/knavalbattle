/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
  Copyright (c) 2007 Riccardo Iaconelli <ruphy@fsfe.org>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "mainwindow.h"


#include <KAction>
#include <KActionCollection>
#include <KIcon>
#include <KLocalizedString>
#include <KStandardAction>
#include <KStandardGameAction>
#include <KStatusBar>
#include <KToggleAction>

#include "playfield.h"
#include "settings.h"
#include "simplemenu.h"
#include "animator.h"

MainWindow::MainWindow(const KUrl& url)
{
    m_main = new PlayField(this, statusBar());
    
    setCentralWidget(m_main);

    Kg::difficulty()->addStandardLevelRange(
        KgDifficultyLevel::Easy, KgDifficultyLevel::Hard, //range
        KgDifficultyLevel::Hard //default
    );
    KgDifficultyGUI::init(this);

    connect(Kg::difficulty(),   SIGNAL  (currentLevelChanged(const KgDifficultyLevel*)),
            m_main,             SLOT    (levelChanged()));

    setupActions();

    connect(m_main, SIGNAL(welcomeScreen()), this, SLOT(welcomeScreen()));
    connect(m_main, SIGNAL(placeShips()), this, SLOT(startingGame()));
    
    m_main->newGame();

    if(! url.isEmpty() )
        m_main->createClient(url);
}

void MainWindow::setupActions()
{
    KStandardGameAction::gameNew(m_main, SLOT(newGame()), actionCollection());
    KStandardGameAction::restart(m_main, SLOT(restart()), actionCollection());     
    KStandardGameAction::highscores(m_main, SLOT(highscores()), actionCollection());
    
    KStandardGameAction::quit(this, SLOT(close()), actionCollection());
    
    KAction* action;
    action = new KAction(KIcon( QLatin1String( SimpleMenu::iconLocal) ), i18n("&Single Player"), this);
    actionCollection()->addAction("game_local", action);
    connect(action, SIGNAL(triggered()), m_main, SLOT(localGame()));
    action = new KAction(KIcon( QLatin1String( SimpleMenu::iconServer) ), i18n("&Host Game..."), this);
    actionCollection()->addAction("game_create_server", action);
    connect(action, SIGNAL(triggered()), m_main, SLOT(createServer()));
    action = new KAction(KIcon( QLatin1String( SimpleMenu::iconClient) ), i18n("&Connect to Game..."), this);
    actionCollection()->addAction("game_create_client", action);
    connect(action, SIGNAL(triggered()), m_main, SLOT(createClient()));
    // settings
    action = new KAction(i18n("Change &Nickname..."), this);
    actionCollection()->addAction("options_nickname", action);
    connect(action, SIGNAL(triggered()), m_main, SLOT(changeNick()));
    action = new KToggleAction(i18n("&Play Sounds"), this);
    actionCollection()->addAction("options_sounds", action);
    connect(action, SIGNAL(triggered(bool)), m_main, SLOT(toggleSounds(bool)));
    // This action will be disabled when a game is being run
    action = new KToggleAction(i18n("&Adjacent Ships"), this);
    action->setChecked(Settings::adjacentShips());
    actionCollection()->addAction("options_adjacent", action);
    connect(action, SIGNAL(triggered(bool)), m_main, SLOT(toggleAdjacent(bool)));
    // config end of game message
    action = new KToggleAction(i18n("Show End-of-Game Message"), this);
    action->setChecked(true);
    actionCollection()->addAction("options_show_endgame_message", action);
    connect(action, SIGNAL(triggered(bool)), m_main, SLOT(toggleEndOfGameMessage(bool)));
    // actions for grid
    action = new KToggleAction(i18n("Show &Left Grid"), this);
    action->setChecked(true);
    actionCollection()->addAction("options_showleftgrid", action);
    connect(action, SIGNAL(triggered(bool)), m_main, SLOT(toggleLeftGrid(bool)));
    action = new KToggleAction(i18n("Show &Right Grid"), this);
    action->setChecked(true);
    actionCollection()->addAction("options_showrightgrid", action);
    connect(action, SIGNAL(triggered(bool)), m_main, SLOT(toggleRightGrid(bool)));
    
    setupGUI();
}

void MainWindow::startingGame()
{
    stateChanged("playing");
}

void MainWindow::welcomeScreen()
{
    stateChanged("playing", KXMLGUIClient::StateReverse);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Animator::instance()->stop();
}

#include "mainwindow.moc"

