/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "simplemenu.h"

#include <QTcpSocket>
#include <QTcpServer>

#include <KLocalizedString>
#include <KIcon>

#include "button.h"
#include "chatwidget.h"
#include "generalcontroller.h"
#include "networkdialog.h"
#include "playerentity.h"
#include "welcomescreen.h"

SimpleMenu::SimpleMenu(QWidget* parent, WelcomeScreen* screen)
: QObject(parent)
, m_screen(screen)
, m_socket(0)
, m_state(READY)
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

void SimpleMenu::finalize(State state)
{
    m_state = state;
//     m_screen->fadeOut();
    emit done();
}

void SimpleMenu::localGame()
{
    finalize(DONE_LOCAL_GAME);
}

void SimpleMenu::createServer()
{
    QWidget* parent_widget = qobject_cast<QWidget*>(parent());
    Q_ASSERT(parent_widget);
    NetworkDialog dialog(false, parent_widget);
    if (dialog.exec() == QDialog::Accepted) {
        m_nickname = dialog.nickname();
        QTcpServer* server = new QTcpServer;
        connect(server, SIGNAL(newConnection()), this, SLOT(processServerConnection()));
        server->listen(QHostAddress::Any, static_cast<quint16>(dialog.port()));
        
        // TODO: show feedback: "waiting for an incoming connection"
    }
}

void SimpleMenu::processServerConnection()
{
    QTcpServer* server = qobject_cast<QTcpServer*>(sender());
    Q_ASSERT(server);
    
    m_socket = server->nextPendingConnection();
    Q_ASSERT(m_socket);
    
    // refuse all other connections
    while (server->hasPendingConnections()) {
        delete server->nextPendingConnection();
    }
    
    // reparent socket, so that we can safely destroy the server
    m_socket->setParent(this);
    delete server;
    
    // we're done
    server->deleteLater();    
    finalize(DONE_SERVER);
}

void SimpleMenu::createClient()
{
    QWidget* parent_widget = qobject_cast<QWidget*>(parent());
    Q_ASSERT(parent_widget);
    NetworkDialog dialog(true, parent_widget);
    if (dialog.exec() == QDialog::Accepted) {
        m_socket = new QTcpSocket(this);
        m_nickname = dialog.nickname();
        connect(m_socket, SIGNAL(connected()), this, SLOT(clientOK()));
        connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(clientError()));
        m_socket->connectToHost(dialog.hostname(), dialog.port());
        
        // TODO: show feedback: connecting...
    }
}

void SimpleMenu::clientError()
{
    delete m_socket;
    m_socket = 0;
    m_nickname = "";
    // TODO: display error message
}

void SimpleMenu::clientOK()
{
    finalize(DONE_CLIENT);
}

void SimpleMenu::setupController(GeneralController* controller, SeaView* sea, ChatWidget* chat)
{
    switch (m_state) {
    case DONE_LOCAL_GAME:
        controller->createPlayer(Sea::Player(0), sea, chat, "");
        controller->createAI(Sea::Player(1));
        controller->start(sea);
        chat->hide();
        
//         QObject::connect(m_chat, SIGNAL(message(QString, QString)),
//             entity, SIGNAL(chat(QString, QString)));
        break;
    case DONE_SERVER: {
        Q_ASSERT(m_socket);
        PlayerEntity* player = controller->createPlayer(Sea::Player(0), sea, chat, m_nickname);
        controller->createRemotePlayer(Sea::Player(1), m_socket, false);
        controller->start(sea);
        chat->show();
        connect(chat, SIGNAL(message(QString, QString)),
            player, SIGNAL(chat(QString, QString)));
        break;
    }
    case DONE_CLIENT: {
        Q_ASSERT(m_socket);
        PlayerEntity* player = controller->createPlayer(Sea::Player(0), sea, chat, m_nickname);
        controller->createRemotePlayer(Sea::Player(1), m_socket, true);
        controller->start(sea);
        chat->show();
        connect(chat, SIGNAL(message(QString, QString)),
            player, SIGNAL(chat(QString, QString)));
        break;
    }
    default:
        break;
    }
}

#include "simplemenu.moc"

