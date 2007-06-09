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
#include <QLayout>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSignalMapper>
#include <kscoredialog.h>
#include <klocale.h>
#include <kmessagebox.h>

#include "seaview.h"
#include "generalcontroller.h"
#include "stats.h"
#include "audioplayer.h"
#include "chatwidget.h"
#include "playerentity.h"
#include "welcomescreen.h"
#include "gamechooser.h"

PlayField::PlayField(QWidget* parent)
: QWidget(parent)
, m_human_player(-1)
{
    setMinimumSize(QSize(400, 300));
    QVBoxLayout* layout = new QVBoxLayout;
    m_sea = new SeaView(this);
    layout->addWidget(m_sea);
    
    m_chat = new ChatWidget(this);
    m_chat->hide();
    layout->addWidget(m_chat);
    
    setLayout(layout);
        
    m_controller = 0;
    m_human_player = 0;
    m_chooser = 0;
    
    m_player = new AudioPlayer(this);

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
    delete m_controller;
    m_controller = new GeneralController(this, m_player);
    connect(m_controller, SIGNAL(gameOver(Sea::Player)),
            this, SLOT(gameOver(Sea::Player)));
            
    m_chooser->setupController(m_controller, m_sea, m_chat);
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
    delete m_chooser;
    
    m_chat->hide();
    m_sea->screen(Sea::Player(0))->show();
    m_sea->screen(Sea::Player(1))->show();
    
    m_chooser = new GameChooser(this, 
        m_sea->screen(Sea::Player(0)),
        m_sea->screen(Sea::Player(1)));
    connect(m_chooser, SIGNAL(done()), this, SLOT(setupController()));
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
    if (m_human_player == winner) {
        const Stats* stats = m_controller->stats();
        if (stats) {
            KScoreDialog::FieldInfo info;
//             info[KScoreDialog::Name] = "my nick";
            info[KScoreDialog::Score].setNum(stats->score());
            info[KScoreDialog::Custom1] = QString::number(stats->shots());
            info[KScoreDialog::Custom2] = QString::number(stats->hits());
            info[KScoreDialog::Custom3] = QString::number(stats->misses());
        
            kDebug() << "score = " << stats->score() << endl;
            if (m_highscores->addScore(info)) {
                highscores();
                return;
            }
        }
        
        KMessageBox::information(this, i18n("You win!"));
    }
    else {
        KMessageBox::information(this, i18n("You lose."));
    }
    
    // When we have finished, show again the welcome screen
    emit gameFinished();
}



#include "playfield.moc"


