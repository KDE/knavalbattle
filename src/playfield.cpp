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

#include <QResizeEvent>
#include <QLabel>
#include <QLayout>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSignalMapper>
#include <QStatusBar>

#include <KLocale>
#include <KMessageBox>
#include <KScoreDialog>

#include "seaview.h"
#include "controller.h"
#include "stats.h"
#include "audioplayer.h"
#include "chatwidget.h"
#include "playerentity.h"
#include "welcomescreen.h"
#include "simplemenu.h"

static const int MINIMUM_HEIGHT = 400;

PlayField::PlayField(QWidget* parent, QStatusBar* sbar)
: QWidget(parent)
, m_status_bar(sbar)
{
    setMinimumSize(static_cast<int>(MINIMUM_HEIGHT * 1.8), MINIMUM_HEIGHT);
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
    
    m_player = 0;//new AudioPlayer(this);

    m_highscores = new KScoreDialog(
        KScoreDialog::Name | KScoreDialog::Score | 
        KScoreDialog::Custom1 | 
        KScoreDialog::Custom2 | 
        KScoreDialog::Custom3, 
        this);
    m_highscores->addField(KScoreDialog::Custom1, i18n("Shots"), "shots");
    m_highscores->addField(KScoreDialog::Custom2, i18n("Hits"), "hits");
    m_highscores->addField(KScoreDialog::Custom3, i18n("Misses"), "water");
    
    
    newGame();
}

PlayField::~PlayField()
{
    // controller assumes that the view is still valid
    // when it is destroyed
    delete m_controller;
}

void PlayField::setupController()
{
    // remove welcome screen
    m_sea->screen(Sea::Player(0))->fadeOut();
    m_sea->screen(Sea::Player(1))->fadeOut();

    delete m_controller;
    m_controller = new Controller(this, m_player);
    connect(m_controller, SIGNAL(gameOver(Sea::Player)),
            this, SLOT(gameOver(Sea::Player)));
            
    m_menu->setupController(m_controller, m_sea, m_chat, m_status_bar);
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
}

void PlayField::restart()
{
    endGame();
    setupController();
}

// void PlayField::newSimulation()
// {
//     m_server->close();
//     setupController();
//     m_human_player = -1;
//     m_controller->createAI(Sea::Player(0));
//     m_controller->createAI(Sea::Player(1));
//     m_controller->start(m_sea);
//     m_chat->hide();
// }

// void PlayField::newServer()
// {
//     if (!m_server->isListening()) {
//         setupController();
//         m_server->listen(QHostAddress::Any, 54321);
//     }
// }

// void PlayField::newClient()
// {
//     if (!m_client) {
//         m_server->close();
//         setupController();
//         m_client = new QTcpSocket;
//         connect(m_client, SIGNAL(connected()), this, SLOT(clientConnected()));
//         m_client->connectToHost("localhost", 54321);
//     }
// }

// void PlayField::acceptClient()
// {
//     QTcpSocket* socket = m_server->nextPendingConnection();
//     if (socket) {
//         m_human_player = 0;
//         PlayerEntity* player = m_controller->createPlayer(Sea::Player(0), m_sea, 
//                                                           m_chat, "serverdude");
//         connect(m_chat, SIGNAL(message(QString, QString)),
//             player, SIGNAL(chat(QString, QString)));
//         m_controller->createRemotePlayer(Sea::Player(1), socket, false);
//         m_controller->start(m_sea);
//         m_server->close();
//         m_chat->show();
//     }
// }

// void PlayField::clientConnected()
// {
//     if (m_client) {
//         // TODO restore chat
// //         connect(m_chat, SIGNAL(message(QString, QString)),
// //             player, SIGNAL(chat(QString, QString)));
//         m_controller->createRemotePlayer(Sea::Player(1), m_client, true);
//         m_controller->start(m_sea);
//         m_server->close();
//         m_chat->show();
//     }
//     m_client = 0;
// }

void PlayField::highscores()
{
    m_highscores->exec();
}

void PlayField::gameOver(Sea::Player winner)
{
    if (winner == Sea::Player(0)) {
//         const Stats* stats = m_stats_widgets[0]->stats();
//        
//         if (stats) {
//             KScoreDialog::FieldInfo info;
//             info[KScoreDialog::Name] = m_stats_widgets[0]->nick();
//             info[KScoreDialog::Score].setNum(stats->score());
//             info[KScoreDialog::Custom1] = QString::number(stats->shots());
//             info[KScoreDialog::Custom2] = QString::number(stats->hits());
//             info[KScoreDialog::Custom3] = QString::number(stats->misses());
//         
//             kDebug() << "score =" << stats->score();
//             if (m_highscores->addScore(info, KScoreDialog::AskName)) {
//                 highscores();
//                 return;
//             }
//         }
        
        m_status_bar->showMessage(i18n("You win!"));
    }
    else {
        m_status_bar->showMessage(i18n("You lose."));
    }
    
    // When we have finished, show again the welcome screen
    emit gameFinished();
}

void PlayField::runGGZ(int fd)
{
    m_menu->runGGZ(fd);
}


#include "playfield.moc"


