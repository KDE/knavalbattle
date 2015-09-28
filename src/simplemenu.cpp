/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "simplemenu.h"

#include <QTcpSocket>
#include <QIcon>

#include <KLocalizedString>

#include "button.h"
#include "chatwidget.h"
#include "controller.h"
#include "networkdialog.h"
#include "playerentity.h"
#include "protocol.h"
#include "networkentity.h"
#include "seaview.h"
#include "settings.h"
#include "aientity.h"
#include "welcomescreen.h"

const char* SimpleMenu::iconLocal = "user-identity";
const char* SimpleMenu::iconServer = "network-server";
const char* SimpleMenu::iconClient = "network-connect";

SimpleMenu::SimpleMenu(QWidget* parent, WelcomeScreen* screen)
: QObject(parent)
, m_screen(screen)
, m_protocol(0)
, m_state(READY)
, m_player1(0)
, m_player2(0)
{
    if (m_screen) {    
        // create buttons
        m_local_game_btn = m_screen->addButton(0, 0, QIcon::fromTheme( QLatin1String( iconLocal) ), i18n("Single Player" ));
        m_server_btn = m_screen->addButton(0, 1, QIcon::fromTheme( QLatin1String( iconServer) ), i18n("Host Network Game" ));
        m_client_btn = m_screen->addButton(0, 2, QIcon::fromTheme( QLatin1String( iconClient) ), i18n("Connect to Network Game" ));
        
        // create connections
        connect(m_local_game_btn, &Button::clicked, this, &SimpleMenu::localGame);
        connect(m_server_btn, &Button::clicked, this, &SimpleMenu::createServer);
        connect(m_client_btn, &Button::clicked, this, &SimpleMenu::createClient);
    }
}

void SimpleMenu::finalize(State state, const QString& nickname, QTcpSocket* socket)
{
    m_state = state;
    m_nickname = nickname;
    if (socket) {
        m_protocol = new Protocol(socket);
        m_protocol->setParent(this);
    }
    else {
        m_protocol = 0;
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
    if (dialog.exec()) {
        finalize(DONE_SERVER, dialog.nickname(), dialog.socket());
    }
}

void SimpleMenu::createClientWithUrl(const QUrl& url)
{
    QWidget* parent_widget = qobject_cast<QWidget*>(parent());
    Q_ASSERT(parent_widget);
    NetworkDialog dialog(true, parent_widget,&url);
    if (dialog.exec() == QDialog::Accepted) {
        finalize(DONE_CLIENT, dialog.nickname(), dialog.socket());
    }
}

void SimpleMenu::createClient()
{
    QWidget* parent_widget = qobject_cast<QWidget*>(parent());
    Q_ASSERT(parent_widget);
    NetworkDialog dialog(true, parent_widget);
    if (dialog.exec() == QDialog::Accepted) {
        finalize(DONE_CLIENT, dialog.nickname(), dialog.socket());
    }
}

void SimpleMenu::setupController(Controller* controller, Entity* old_opponent, SeaView* sea, 
    ChatWidget* chat, bool ask)
{
    switch (m_state) {
    case DONE_LOCAL_GAME: {
        m_player1 = controller->createPlayer(Sea::Player(0), sea, chat, m_nickname);
        sea->setStats(Sea::Player(0), "score_mouse", m_nickname, m_player1->stats());
        m_player2 = controller->createAI(Sea::Player(1), sea);
        sea->setStats(Sea::Player(1), "score_ai", 
                      "Computer", m_player2->stats());
        chat->hide();
        break;
    }
    case DONE_SERVER: {
        Q_ASSERT(m_protocol);
        m_player1 = controller->createPlayer(Sea::Player(0), sea, chat, m_nickname);
        sea->setStats(Sea::Player(0), "score_mouse", 
                      m_nickname, m_player1->stats());
        m_player2 = controller->createRemotePlayer(Sea::Player(1), sea, m_protocol, false);
        if (old_opponent) {
            m_player1->setCompatibilityLevel(old_opponent->compatibilityLevel());
            m_player2->setCompatibilityLevel(old_opponent->compatibilityLevel());
            m_player2->setNick(old_opponent->nick());
        }
        sea->setStats(Sea::Player(1), "score_network", 
                      m_player2->nick().isEmpty() ? i18n("Remote player") : m_player2->nick(),
                      m_player2->stats());
        chat->bindTo(m_player1);
        break;
    }
    case DONE_CLIENT: {
        Q_ASSERT(m_protocol);
        m_player1 = controller->createPlayer(Sea::Player(0), sea, chat, m_nickname);
        sea->setStats(Sea::Player(0), "score_mouse", 
                      m_nickname, m_player1->stats());
        m_player2 = controller->createRemotePlayer(Sea::Player(1), sea, m_protocol, true);
        if (old_opponent) {
            m_player1->setCompatibilityLevel(old_opponent->compatibilityLevel());
            m_player2->setCompatibilityLevel(old_opponent->compatibilityLevel());
            m_player2->setNick(old_opponent->nick());
        }
        sea->setStats(Sea::Player(1), "score_network", 
                      m_player2->nick().isEmpty() ? i18n("Remote player") : m_player2->nick(),
                      m_player2->stats());
        chat->bindTo(m_player1);
        break;
    }
    default:
        return;
    }
        
    controller->start(sea, ask);
}


