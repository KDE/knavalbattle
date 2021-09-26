/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SIMPLEMENU_H
#define SIMPLEMENU_H

#include <QObject>

class WelcomeScreen;
class Controller;
class SeaView;
class ChatWidget;
class Button;
class QTcpSocket;
class Entity;
class Protocol;
class QUrl;

class SimpleMenu : public QObject
{
Q_OBJECT
    WelcomeScreen* m_screen;
    
    Button* m_local_game_btn;
    Button* m_server_btn;
    Button* m_client_btn;
    
    Protocol* m_protocol;
    QString m_nickname;
    
    enum State
    {
        READY,
        DONE_LOCAL_GAME,
        DONE_SERVER,
        DONE_CLIENT,
    } m_state;
    
    Entity* m_player1;
    Entity* m_player2;
    
    void finalize(State, const QString& nick, QTcpSocket* socket = nullptr);
public:
    SimpleMenu(QWidget* parent, WelcomeScreen* screen);
    
    void setupController(Controller* controller, Entity* old_opponent,
        SeaView* sea, ChatWidget* chat);
    void createClientWithUrl(const QUrl& url);
    
    Entity* player(int p) { return p == 0 ? m_player1 : m_player2; }
    
    static const char* iconLocal;
    static const char* iconServer;
    static const char* iconClient;
public Q_SLOTS:
    void localGame();
    
    void createServer();
    void createClient();
Q_SIGNALS:
    void done();
};

#endif // SIMPLEMENU_H
