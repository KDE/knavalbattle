/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "simplemenu.h"

#include <QLabel>
#include <QStatusBar>
#include <QTcpSocket>
#include <QTcpServer>

#include <KLocalizedString>
#include <KIcon>

#include <kggzmod/module.h>
#include <kggzmod/player.h>

#include "button.h"
#include "chatwidget.h"
#include "controller.h"
#include "networkdialog.h"
#include "playerentity.h"
#include "networkentity.h"
#include "seaview.h"
#include "settings.h"
#include "aientity.h"
#include "welcomescreen.h"

SimpleMenu::SimpleMenu(QWidget* parent, WelcomeScreen* screen)
: QObject(parent)
, m_screen(screen)
, m_socket(0)
, m_state(READY)
, m_player1(0)
, m_player2(0)
{
    Q_ASSERT(m_screen);
    
    // create buttons
    m_local_game_btn = m_screen->addButton(0, 0, KIcon("user-female"), i18n("Single player"));
    m_server_btn = m_screen->addButton(0, 1, KIcon("roll"), i18n("Host network game"));
    m_client_btn = m_screen->addButton(0, 2, KIcon("network"), i18n("Connect to network game"));
    
    // create connections
    connect(m_local_game_btn, SIGNAL(clicked()),
        this, SLOT(localGame()));
    connect(m_server_btn, SIGNAL(clicked()),
        this, SLOT(createServer()));
    connect(m_client_btn, SIGNAL(clicked()),
        this, SLOT(createClient()));
}

void SimpleMenu::finalize(State state, const QString& nickname, QTcpSocket* socket)
{
    m_state = state;
    m_nickname = nickname;
    m_socket = socket;
    if (m_socket) {
        m_socket->setParent(this);
    }
    emit done();
}

void SimpleMenu::localGame()
{
    finalize(DONE_LOCAL_GAME, Settings::findNick());
}

void SimpleMenu::createServer()
{
    QWidget* parent_widget = qobject_cast<QWidget*>(parent());
    Q_ASSERT(parent_widget);
    NetworkDialog dialog(false, parent_widget);
    connect(&dialog, SIGNAL(ok()), this, SLOT(createServerOk()));
    if (dialog.exec()) {
        finalize(DONE_SERVER, dialog.nickname(), dialog.socket());
    }
}

void SimpleMenu::createClient()
{
    QWidget* parent_widget = qobject_cast<QWidget*>(parent());
    Q_ASSERT(parent_widget);
    NetworkDialog dialog(true, parent_widget);
    if (dialog.exec() == QDialog::Accepted) {
        finalize(DONE_SERVER, dialog.nickname(), dialog.socket());
    }
}

void SimpleMenu::setupController(Controller* controller, SeaView* sea, 
    ChatWidget* chat, QStatusBar* sbar, bool restart)
{
    switch (m_state) {
    case DONE_LOCAL_GAME: {
        QString nick = Settings::findNick();
        m_player1 = controller->createPlayer(Sea::Player(0), sea, chat, m_nickname);
        sea->setStats(Sea::Player(0), "score_mouse", nick, m_player1->stats());
        m_player2 = controller->createAI(Sea::Player(1));
        sea->setStats(Sea::Player(1), "score_ai", 
                      "Computer", m_player2->stats());
        chat->hide();
        break;
    }
    case DONE_SERVER: {
        Q_ASSERT(m_socket);
        m_player1 = controller->createPlayer(Sea::Player(0), sea, chat, m_nickname);
        sea->setStats(Sea::Player(0), "score_mouse", 
                      m_nickname, m_player1->stats());
        m_player2 = controller->createRemotePlayer(Sea::Player(1), m_socket, false);
        sea->setStats(Sea::Player(1), "score_network", 
                      "Remote player", m_player2->stats());
        chat->bindTo(m_player1);
        break;
    }
    case DONE_CLIENT: {
        Q_ASSERT(m_socket);
        m_player1 = controller->createPlayer(Sea::Player(0), sea, chat, m_nickname);
        sea->setStats(Sea::Player(0), "score_mouse", 
                      m_nickname, m_player1->stats());
        m_player2 = controller->createRemotePlayer(Sea::Player(1), m_socket, true);
        sea->setStats(Sea::Player(1), "score_network", 
                      "Remote player", m_player2->stats());
        chat->bindTo(m_player1);
        break;
    }
    case DONE_GGZ_CLIENT: {
        KGGZMod::Player *player = KGGZMod::Module::instance()->self();
        if(player) {
            kDebug() << "PLAYER-exists";
            m_nickname = player->name();
            kDebug() << "nickname" << m_nickname;
        } else {
            kDebug() << "PLAYER-does-not-exist";
            return;
        }

        int seat = player->seat();
        int oppseat = 1 - seat;

        Q_ASSERT(m_socket);
        m_player1 = controller->createPlayer(Sea::Player(seat), sea, chat, m_nickname);
        m_player2 = controller->createRemotePlayer(Sea::Player(oppseat), m_socket, true);
        chat->bindTo(m_player1);
        break;
    }
    default:
        return;
    }
    
    connect(m_player1, SIGNAL(message(const QString&)),
        sbar, SLOT(showMessage(const QString&)));
        
    controller->start(sea, restart);
}

void SimpleMenu::runGGZ(int fd)
{
    m_state = DONE_GGZ_CLIENT;
    m_socket = new QTcpSocket();
    m_socket->setSocketDescriptor(fd);
}

#include "simplemenu.moc"

