/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
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
class QStatusBar;
class QLabel;

class SimpleMenu : public QObject
{
Q_OBJECT
    WelcomeScreen* m_screen;
    
    Button* m_local_game_btn;
    Button* m_server_btn;
    Button* m_client_btn;
    
    QTcpSocket* m_socket;
    QString m_nickname;
    
    enum State
    {
        READY,
        DONE_LOCAL_GAME,
        DONE_SERVER,
        DONE_CLIENT,
        DONE_GGZ_CLIENT
    } m_state;
    void finalize(State);
public:
    SimpleMenu(QWidget* parent, WelcomeScreen* screen);
    
    void setupController(Controller* controller,
        SeaView* sea, ChatWidget* chat, QStatusBar* sbar);
    void runGGZ(int fd);
private slots:
    void localGame();
    
    void createServer();
    void processServerConnection();

    void createClient();
    void clientError();
    void clientOK();
signals:
    void done();
};

#endif // SIMPLEMENU_H
