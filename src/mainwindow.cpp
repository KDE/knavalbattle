/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
  Copyright (c) 2007 Riccardo Iaconelli <ruphy@fsfe.org>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "mainwindow.h"

#include <QAction>
#include <QIcon>

#include <KActionCollection>
#include <KLocalizedString>
#include <KStandardAction>
#include <KStandardGameAction>
#include <QStatusBar>
#include <KToggleAction>

#include "playfield.h"
#include "settings.h"
#include "simplemenu.h"
#include "animator.h"

MainWindow::MainWindow(const QUrl& url)
{
    m_main = new PlayField(this, statusBar());
    
    setCentralWidget(m_main);

    Kg::difficulty()->addStandardLevelRange(
        KgDifficultyLevel::Easy, KgDifficultyLevel::Hard, //range
        KgDifficultyLevel::Hard //default
    );
    KgDifficultyGUI::init(this);

    connect(Kg::difficulty(), &KgDifficulty::currentLevelChanged, m_main, &PlayField::levelChanged);

    setupActions();

    connect(m_main, &PlayField::welcomeScreen, this, &MainWindow::welcomeScreen);
    connect(m_main, &PlayField::placeShips, this, &MainWindow::startingGame);
    
    m_main->newGame();

    if(! url.isEmpty() )
        m_main->createClientWithUrl(url);
}

void MainWindow::setupActions()
{
    KStandardGameAction::gameNew(m_main, SLOT(newGame()), actionCollection());
    KStandardGameAction::restart(m_main, SLOT(restart()), actionCollection());
    KStandardGameAction::highscores(m_main, SLOT(highscores()), actionCollection());
    
    KStandardGameAction::quit(this, SLOT(close()), actionCollection());
    
    QAction* action;
    action = new QAction(i18n("&Single Player"), this);
    action->setIcon(QIcon::fromTheme( QLatin1String( SimpleMenu::iconLocal)));
    actionCollection()->addAction(QLatin1Literal("game_local"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::localGame);
    action = new QAction(i18n("&Host Game..."), this);
    action->setIcon(QIcon::fromTheme( QLatin1String( SimpleMenu::iconServer)));
    actionCollection()->addAction(QLatin1Literal("game_create_server"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::createServer);
    action = new QAction(i18n("&Connect to Game..."), this);
    action->setIcon(QIcon::fromTheme( QLatin1String( SimpleMenu::iconClient))),
    actionCollection()->addAction(QLatin1Literal("game_create_client"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::createClient);
    // settings
    action = new QAction(i18n("Change &Nickname..."), this);
    actionCollection()->addAction(QLatin1Literal("options_nickname"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::changeNick);
    action = new KToggleAction(i18n("&Play Sounds"), this);
    actionCollection()->addAction(QLatin1Literal("options_sounds"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::toggleSounds);
    // This action will be disabled when a game is being run
    action = new KToggleAction(i18n("&Adjacent Ships"), this);
    action->setChecked(Settings::adjacentShips());
    actionCollection()->addAction(QLatin1Literal("options_adjacent"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::toggleAdjacent);
    // This action will be disabled when a game is being run
    action = new KToggleAction(i18n("&Multiple Ships"), this);
    action->setChecked(Settings::severalShips());
    actionCollection()->addAction(QLatin1Literal("options_multiple_ships"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::toggleMultiple);
    // config end of game message
    action = new KToggleAction(i18n("Show End-of-Game Message"), this);
    action->setChecked(true);
    actionCollection()->addAction(QLatin1Literal("options_show_endgame_message"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::toggleEndOfGameMessage);
    // actions for grid
    action = new KToggleAction(i18n("Show &Left Grid"), this);
    action->setChecked(true);
    actionCollection()->addAction(QLatin1Literal("options_showleftgrid"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::toggleLeftGrid);
    action = new KToggleAction(i18n("Show &Right Grid"), this);
    action->setChecked(true);
    actionCollection()->addAction(QLatin1Literal("options_showrightgrid"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::toggleRightGrid);
    
    setupGUI();
}

void MainWindow::startingGame()
{
    stateChanged(QLatin1Literal("playing"));
}

void MainWindow::welcomeScreen()
{
    stateChanged(QLatin1Literal("playing"), KXMLGUIClient::StateReverse);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Animator::instance()->stop();
}

