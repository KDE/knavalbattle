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
#include "ships.h"

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
    BattleShipsConfiguration m_battle_ships_configuration;

    void startGame();
    void endGame();
    Controller* createController();
    SimpleMenu* createAuxMenu();
public:
    PlayField(QWidget* parent, QStatusBar*);
    ~PlayField();
    void createClientWithUrl(const QUrl& url);
public slots:
    void highscores();
    void gameOver(Sea::Player winner);
    void setupController();

    void newGame();
    void restart();
    void changeNick();
    void toggleSounds(bool);
    void toggleAdjacent(bool);
    void toggleMultiple(bool);
    void restartRequested();
    void startPlacingShips();
    void restartPlacingShips(Sea::Player player);
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
    void abortGame(Sea::Player player);
    void welcomeScreen();
    void placeShips();
};

#endif // PLAYFIELD_H

