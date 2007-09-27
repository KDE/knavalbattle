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

void PlayField::setupController(bool restart)
{
    // remove welcome screen
    m_sea->screen(Sea::Player(0))->fadeOut();
    m_sea->screen(Sea::Player(1))->fadeOut();

    delete m_controller;
    m_controller = new Controller(this, m_player);
    connect(m_controller, SIGNAL(gameOver(Sea::Player)),
            this, SLOT(gameOver(Sea::Player)));
    connect(m_controller, SIGNAL(restartRequested()),
            this, SLOT(restartRequested()));
            
    m_menu->setupController(m_controller, m_sea, m_chat, m_status_bar, restart);
    emit startingGame();
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
    
    m_chat->hide();
    m_sea->screen(Sea::Player(0))->show();
    m_sea->screen(Sea::Player(1))->show();
    
    m_menu = new SimpleMenu(this, m_sea->screen(Sea::Player(0)));
    connect(m_menu, SIGNAL(done()), this, SLOT(setupController()));
    emit welcomeScreen();
}

void PlayField::restart()
{
    endGame();
    setupController(true);
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
            KScoreDialog::FieldInfo info;
            info[KScoreDialog::Name] = m_menu->player(0)->nick();
            info[KScoreDialog::Score].setNum(stats->score());
            info[KScoreDialog::Custom2] = QString::number(stats->hits());
            info[KScoreDialog::Custom3] = QString::number(stats->misses());
        
            kDebug() << "score =" << stats->score();
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

void PlayField::updatePreferences()
{
    m_player->setActive(Settings::enableSounds());
}

void PlayField::runGGZ(int fd)
{
    m_menu->runGGZ(fd);
}

void PlayField::restartRequested() {
    int ans = KMessageBox::questionYesNo(this, i18n("Restart game"),
                    i18n("Your opponent has requested to restart the game. Do you accept?"));
    if (ans == KMessageBox::Yes) {
        restart();
    }
}


#include "playfield.moc"


