/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#include <QWidget>
#include "sea.h"

class SeaView;
class GeneralController;
class KScoreDialog;
class QTcpServer;
class QTcpSocket;
class AudioPlayer;
class ChatWidget;
class QSignalMapper;

class PlayField : public QWidget
{
Q_OBJECT
    SeaView* m_sea;
    GeneralController* m_controller;
    KScoreDialog* m_highscores;
    QTcpServer* m_server;
    QTcpSocket* m_client;
    AudioPlayer* m_player;
    ChatWidget* m_chat;
    int m_human_player;
    QSignalMapper* m_mappers[3];
    
    void setupController();
public:
    PlayField(QWidget* parent);
    ~PlayField();
public slots:
    void newGame();
    void newSimulation();
    void newServer();
    void newClient();
    void highscores();
    void gameOver(Sea::Player winner);
private slots:
    void acceptClient();
    void clientConnected();
    void human(int);
    void ai(int);
    void network(int);
signals:
    void gameFinished();
};

#endif // PLAYFIELD_H

