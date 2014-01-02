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
#include <KgDifficulty>
#include "sea.h"

class SeaView;
class Controller;
class AudioPlayer;
class ChatWidget;
class SimpleMenu;
class QStatusBar;

class PlayField : public QWidget
{
Q_OBJECT
    SeaView* m_seaView;
    Controller* m_controller;
    AudioPlayer* m_player;
    ChatWidget* m_chat;
    SimpleMenu* m_menu;
    QStatusBar* m_status_bar;
    bool m_show_endofgame_message;

    void startGame();
    void endGame();
    void resetupController(bool ask = false);
    Controller* createController();
    SimpleMenu* createAuxMenu();
public:
    PlayField(QWidget* parent, QStatusBar*);
    ~PlayField();
    void createClient(const KUrl& url);
public slots:
    void highscores();
    void gameOver(Sea::Player winner);
    void setupController();

    void newGame();
    void restart(bool ask = true);
    void changeNick();
    void toggleSounds(bool);
    void toggleAdjacent(bool);
    void restartRequested();
    void setCompatibility(int);
    void updateNick(int, const QString&);
    void changeTurn(int);
    void playerReady(int);
    void levelChanged();
    
    void auxMenuDone();
    void localGame();
    void createServer();
    void createClient();

    void toggleEndOfGameMessage(bool show);
    void toggleLeftGrid(bool show);
    void toggleRightGrid(bool show);
signals:
    void gameFinished();
    void welcomeScreen();
    void startingGame();
};

#endif // PLAYFIELD_H

