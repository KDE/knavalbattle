/*
  Copyright (c) 2000-2001 Nikolas Zimmermann <wildfox@kde.org>
            (c) 2000-2001 Daniel Molkentin <molkentin@kde.org>
            (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "playfield.h"

#include <QLabel>
#include <QLayout>
#include <QResizeEvent>
#include <QSignalMapper>
#include <QStatusBar>
#include <QTcpServer>
#include <QTcpSocket>

#include <KInputDialog>
#include <KLocale>
#include <KMessageBox>
#include <KScoreDialog>

#include "aientity.h"
#include "audioplayer.h"
#include "chatwidget.h"
#include "controller.h"
#include "playerentity.h"
#include "seaview.h"
#include "settings.h"
#include "simplemenu.h"
#include "stats.h"
#include "welcomescreen.h"

static const int MINIMUM_HEIGHT = 400;

PlayField::PlayField(QWidget* parent, QStatusBar* sbar)
: QWidget(parent)
, m_status_bar(sbar)
{
    setMinimumSize(static_cast<int>(MINIMUM_HEIGHT * 1.6), MINIMUM_HEIGHT);
    QVBoxLayout* layout = new QVBoxLayout;
    
    m_sea = new SeaView(this);
    layout->addWidget(m_sea, 1);
    
    m_chat = new ChatWidget(this);
    m_chat->hide();
    layout->addWidget(m_chat, 1);
    
    layout->setMargin(0);
//     layout->setSpacing(0);
    setLayout(layout);
        
    m_controller = 0;
    m_menu = 0;
    
    m_player = new AudioPlayer(this);
    m_player->setActive(Settings::enableSounds());

    m_highscores = new KScoreDialog(
        KScoreDialog::Name | KScoreDialog::Score | 
        KScoreDialog::Custom1 | 
        KScoreDialog::Custom2 | 
        KScoreDialog::Custom3, 
        this);
    m_highscores->setConfigGroup(KGameDifficulty::levelString());
    m_highscores->addField(KScoreDialog::Custom1, i18n("Shots"), "shots");
    m_highscores->addField(KScoreDialog::Custom2, i18n("Hits"), "hits");
    m_highscores->addField(KScoreDialog::Custom3, i18n("Misses"), "water");
}

PlayField::~PlayField()
{
    // controller assumes that the view is still valid
    // when it is destroyed
    delete m_controller;
}

Controller* PlayField::createController()
{
    Controller* controller = new Controller(this, m_player);
    connect(controller, SIGNAL(gameOver(Sea::Player)),
            this, SLOT(gameOver(Sea::Player)));
    connect(controller, SIGNAL(restartRequested()),
            this, SLOT(restartRequested()));
    connect(controller, SIGNAL(compatibility(int)),
            this, SLOT(setCompatibility(int)));
    connect(controller, SIGNAL(nickChanged(int, const QString&)),
            this, SLOT(updateNick(int, const QString&)));
    connect(controller, SIGNAL(turnChanged(int)),
            this, SLOT(changeTurn(int)));
    connect(controller, SIGNAL(playerReady(int)),
            this, SLOT(playerReady(int)));
    return controller;
}

void PlayField::setupController()
{
    Animator::instance()->restart();
    m_sea->clear();
    m_chat->hide();
    
    // remove welcome screen
    m_sea->screen(Sea::Player(0))->fadeOut();
    m_sea->screen(Sea::Player(1))->fadeOut();

    delete m_controller;
    m_controller = createController();
    m_menu->setupController(m_controller, 0, m_sea, m_chat, false);
    startGame();
}

void PlayField::resetupController(bool ask)
{
    Entity* old_opponent = m_controller->findEntity(Sea::Player(1));
    if (old_opponent) {
        old_opponent->setParent(0);
    }    
    delete m_controller;

    // create new controller
    m_controller = createController();
    m_menu->setupController(m_controller, old_opponent, 
        m_sea, m_chat, ask);
    
    delete old_opponent;
    
    startGame();
}

void PlayField::endGame()
{
    Animator::instance()->restart();
    delete m_controller;
    m_controller = 0;
    m_sea->clear();
}

void PlayField::newGame()
{
    endGame();
    delete m_menu;
    
    KGameDifficulty::setRunning(false);
    
    m_chat->hide();
    m_sea->screen(Sea::Player(0))->show();
    m_sea->screen(Sea::Player(1))->show();
    
    m_menu = new SimpleMenu(this, m_sea->screen(Sea::Player(0)));
    connect(m_menu, SIGNAL(done()), this, SLOT(setupController()));
    emit welcomeScreen();
}

void PlayField::restart(bool ask)
{
    Animator::instance()->restart();
    m_sea->clear();
    resetupController(ask);
}


void PlayField::highscores()
{
    m_highscores->exec();
}

void PlayField::gameOver(Sea::Player winner)
{
    if (winner == Sea::Player(0)) {
        const Stats* stats = m_menu->player(0)->stats();
       
        if (stats && qobject_cast<const AIEntity*>(m_menu->player(1))) {
            m_highscores->setConfigGroup(KGameDifficulty::levelString());
        
            KScoreDialog::FieldInfo info;
            info[KScoreDialog::Name] = m_menu->player(0)->nick();
            info[KScoreDialog::Score].setNum(stats->score());
            info[KScoreDialog::Custom1] = QString::number(stats->shots());
            info[KScoreDialog::Custom2] = QString::number(stats->hits());
            info[KScoreDialog::Custom3] = QString::number(stats->misses());
        
            if (m_highscores->addScore(info, KScoreDialog::AskName)) {
                highscores();
                return;
            }
        }
        
        m_status_bar->showMessage(i18n("You win!"));
    }
    else {
        m_status_bar->showMessage(i18n("You lose."));
    }
    
    // When we have finished, show again the welcome screen
    emit gameFinished();
}

void PlayField::changeNick()
{
    QString nick = KInputDialog::getText(i18n("Change Nickname"), i18n("Enter new nickname:"), Settings::findNick());
    if (!nick.isEmpty()) {
        Settings::setNickname(nick);
        Settings::self()->writeConfig();
    }
}

void PlayField::toggleSounds(bool enable)
{
    Settings::setEnableSounds(enable);
    Settings::self()->writeConfig();
    m_player->setActive(enable);
}

void PlayField::runGGZ(int fd)
{
    m_menu->runGGZ(fd);
}

void PlayField::restartRequested()
{
    int ans = KMessageBox::questionYesNo(this, i18n("Restart game"),
                    i18n("Your opponent has requested to restart the game. Do you accept?"));
    if (ans == KMessageBox::Yes) {
        restart(false);
    }
}

void PlayField::setCompatibility(int level)
{
    if (level == Entity::COMPAT_KBS3) {
        KMessageBox::information(this, i18n("Your opponent is using pre-KDE4 version of KBattleship. Note that, according to the rules enforced by old clients, ships cannot be placed adjacent to one another."));
    }
}

void PlayField::updateNick(int player, const QString& nick)
{
    m_sea->setNick(Sea::Player(player), nick);
}

void PlayField::changeTurn(int player)
{
    if (player == 0) {
        // local user
        m_status_bar->showMessage(i18n("Enemy has shot. Shoot now!"));
    }
    else {
        // opponent
        m_status_bar->showMessage(i18n("Waiting for enemy to shoot..."));
    }
}

void PlayField::playerReady(int player)
{
    if (player == -1) {
        // game can start
        if (m_controller->turn() == 0) {
            m_status_bar->showMessage(i18n("Ships placed. Now shoot on the enemy field!"));
        }
        else {
            m_status_bar->showMessage(i18n("Waiting for other player to start the game..."));
        }
    }
    else if (player == 0) {
        m_status_bar->showMessage(i18n("Waiting for other player to place his ships..."));
    }
}

void PlayField::startGame()
{
    KGameDifficulty::setRestartOnChange(
        m_controller->hasAI() ? KGameDifficulty::RestartOnChange : KGameDifficulty::NoRestartOnChange);
    KGameDifficulty::setRunning(true);
    m_status_bar->showMessage(i18n("Place your ships. Use the right mouse button to rotate them."));
    emit startingGame();
}


void PlayField::levelChanged(KGameDifficulty::standardLevel level)
{
    Settings::setDifficulty(level);
    Settings::self()->writeConfig();
    if (m_controller && m_controller->hasAI()) {
        restart();
    }
}

SimpleMenu* PlayField::createAuxMenu()
{
    SimpleMenu* menu = new SimpleMenu(this, 0);
    connect(menu, SIGNAL(done()), this, SLOT(auxMenuDone()));
    return menu;
}

void PlayField::auxMenuDone()
{
    kDebug() << "aux menu done";
    SimpleMenu* menu = qobject_cast<SimpleMenu*>(sender());
    if (menu) {
        delete m_menu;
        m_menu = menu;
        setupController();
    }
}

void PlayField::localGame()
{
    createAuxMenu()->localGame();
}

void PlayField::createServer()
{
    createAuxMenu()->createServer();
}

void PlayField::createClient()
{
    createAuxMenu()->createClient();
}

#include "playfield.moc"


