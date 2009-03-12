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

#include <kggzmod/module.h>
#include <kggzmod/player.h>

#include "playfield.h"
#include "settings.h"
#include "simplemenu.h"

MainWindow::MainWindow(const KUrl& url)
: m_mod(0), m_fd(-1), m_ggzsetup(false)
{
    m_main = new PlayField(this, statusBar());
    
    setCentralWidget(m_main);

    setupActions();

    if(KGGZMod::Module::isGGZ())
    {
        kDebug() << "GGZDEBUG: running in GGZ mode...";
        m_mod = new KGGZMod::Module("KBattleship");
        connect(m_mod, SIGNAL(signalError()), SLOT(networkErrorHandler()));
        connect(m_mod, SIGNAL(signalNetwork(int)), SLOT(networkData(int)));
        connect(m_mod, SIGNAL(signalEvent(const KGGZMod::Event&)),
            SLOT(networkEvent(const KGGZMod::Event&)));
        kDebug() << "GGZDEBUG: kggzmod activated";
    }
    
    connect(m_main, SIGNAL(welcomeScreen()), this, SLOT(welcomeScreen()));
    connect(m_main, SIGNAL(startingGame()), this, SLOT(startingGame()));
    
    m_main->newGame();
    
    KGameDifficulty::init(this, m_main, SLOT(levelChanged(KGameDifficulty::standardLevel)));
    KGameDifficulty::setRestartOnChange(KGameDifficulty::RestartOnChange);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Easy);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Medium);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Hard);
    KGameDifficulty::setLevel(KGameDifficulty::standardLevel(Settings::difficulty()));

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
    action = new KAction(KIcon(SimpleMenu::iconLocal), i18n("&Single Player"), this);
    actionCollection()->addAction("game_local", action);
    connect(action, SIGNAL(triggered()), m_main, SLOT(localGame()));
    action = new KAction(KIcon(SimpleMenu::iconServer), i18n("&Host Game..."), this);
    actionCollection()->addAction("game_create_server", action);
    connect(action, SIGNAL(triggered()), m_main, SLOT(createServer()));
    action = new KAction(KIcon(SimpleMenu::iconClient), i18n("&Connect to Game..."), this);
    actionCollection()->addAction("game_create_client", action);
    connect(action, SIGNAL(triggered()), m_main, SLOT(createClient()));
    action = new KAction(i18n("Change &Nickname..."), this);
    actionCollection()->addAction("options_nickname", action);
    connect(action, SIGNAL(triggered()), m_main, SLOT(changeNick()));
    action = new KToggleAction(i18n("&Play Sounds"), this);
    actionCollection()->addAction("options_sounds", action);
    connect(action, SIGNAL(triggered(bool)), m_main, SLOT(toggleSounds(bool)));
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

void MainWindow::networkErrorHandler()
{
    kDebug() << "GGZDEBUG: error";
}

void MainWindow::networkData(int fd)
{
    kDebug() << "GGZDEBUG: data on fd" << fd;
}

void MainWindow::networkEvent(const KGGZMod::Event& event)
{
    kDebug() << "GGZDEBUG: event" << event.type();

    if(event.type() == KGGZMod::Event::server) {
        KGGZMod::ServerEvent se = (KGGZMod::ServerEvent)event;
        m_fd = se.fd();
    }

    if(event.type() == KGGZMod::Event::self) {
        if(!m_ggzsetup) {
            KGGZMod::Player *player = KGGZMod::Module::instance()->self();
            if(player) {
                m_ggzsetup = true;
                m_main->runGGZ(m_fd);
                m_main->setupController();
            }
        }
    }
}

void MainWindow::startingGame()
{
    stateChanged("playing");
}

void MainWindow::welcomeScreen()
{
    stateChanged("playing", KXMLGUIClient::StateReverse);
}


#include "mainwindow.moc"

