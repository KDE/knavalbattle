/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#include <QWidget>

#include <KGameDifficulty>

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
    ~PlayField() override;
    void createClientWithUrl(const QUrl& url);
public Q_SLOTS:
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
Q_SIGNALS:
    void gameFinished();
    void abortGame(Sea::Player player);
    void welcomeScreen();
    void placeShips();
};

#endif // PLAYFIELD_H

