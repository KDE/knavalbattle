/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>
    SPDX-FileCopyrightText: 2007 Riccardo Iaconelli <ruphy@fsfe.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mainwindow.h"

#include <QAction>
#include <QIcon>

#include <KActionCollection>
#include <KLocalizedString>
#include <KStandardAction>
#include <KGameStandardAction>
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

    KGameDifficulty::global()->addStandardLevelRange(
        KGameDifficultyLevel::Easy, KGameDifficultyLevel::Hard, //range
        KGameDifficultyLevel::Hard //default
    );
    KGameDifficultyGUI::init(this);

    connect(KGameDifficulty::global(), &KGameDifficulty::currentLevelChanged, m_main, &PlayField::levelChanged);

    setupActions();

    connect(m_main, &PlayField::welcomeScreen, this, &MainWindow::welcomeScreen);
    connect(m_main, &PlayField::placeShips, this, &MainWindow::startingGame);
    
    m_main->newGame();

    if(! url.isEmpty() )
        m_main->createClientWithUrl(url);
}

void MainWindow::setupActions()
{
    KGameStandardAction::gameNew(m_main, &PlayField::newGame, actionCollection());
    KGameStandardAction::restart(m_main, &PlayField::restart, actionCollection());
    KGameStandardAction::highscores(m_main, &PlayField::highscores, actionCollection());
    
    KGameStandardAction::quit(this, &MainWindow::close, actionCollection());
    
    QAction* action;
    action = new QAction(i18nc("@action", "&Single Player"), this);
    action->setIcon(QIcon::fromTheme( QLatin1String( SimpleMenu::iconLocal)));
    actionCollection()->addAction(QStringLiteral("game_local"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::localGame);
    action = new QAction(i18nc("@action", "&Host Game…"), this);
    action->setIcon(QIcon::fromTheme( QLatin1String( SimpleMenu::iconServer)));
    actionCollection()->addAction(QStringLiteral("game_create_server"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::createServer);
    action = new QAction(i18nc("@action", "&Connect to Game…"), this);
    action->setIcon(QIcon::fromTheme( QLatin1String( SimpleMenu::iconClient))),
    actionCollection()->addAction(QStringLiteral("game_create_client"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::createClient);
    // settings
    action = new QAction(i18nc("@action", "Change &Nickname…"), this);
    actionCollection()->addAction(QStringLiteral("options_nickname"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::changeNick);
    action = new KToggleAction(i18nc("@option:check", "Play Sounds"), this);
    action->setChecked(Settings::enableSounds());
    actionCollection()->addAction(QStringLiteral("options_sounds"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::toggleSounds);
    // This action will be disabled when a game is being run
    action = new KToggleAction(i18nc("@option:check", "&Adjacent Ships"), this);
    action->setChecked(Settings::adjacentShips());
    actionCollection()->addAction(QStringLiteral("options_adjacent"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::toggleAdjacent);
    // This action will be disabled when a game is being run
    action = new KToggleAction(i18nc("@option:check", "&Multiple Ships"), this);
    action->setChecked(Settings::severalShips());
    actionCollection()->addAction(QStringLiteral("options_multiple_ships"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::toggleMultiple);
    // config end of game message
    action = new KToggleAction(i18nc("@option:check", "Show End-of-Game Message"), this);
    action->setChecked(true);
    actionCollection()->addAction(QStringLiteral("options_show_endgame_message"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::toggleEndOfGameMessage);
    // actions for grid
    action = new KToggleAction(i18nc("@option:check", "Show &Left Grid"), this);
    action->setChecked(true);
    actionCollection()->addAction(QStringLiteral("options_showleftgrid"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::toggleLeftGrid);
    action = new KToggleAction(i18nc("@option:check", "Show &Right Grid"), this);
    action->setChecked(true);
    actionCollection()->addAction(QStringLiteral("options_showrightgrid"), action);
    connect(action, &QAction::triggered, m_main, &PlayField::toggleRightGrid);
    
    setupGUI();
}

void MainWindow::startingGame()
{
    stateChanged(QStringLiteral("playing"));
}

void MainWindow::welcomeScreen()
{
    stateChanged(QStringLiteral("playing"), KXMLGUIClient::StateReverse);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    Animator::instance()->stop();
}

#include "moc_mainwindow.cpp"
