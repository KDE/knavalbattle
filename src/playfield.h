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
class Controller;
class KScoreDialog;
class AudioPlayer;
class ChatWidget;
class SimpleMenu;
class QStatusBar;
class StatsWidget;

class PlayField : public QWidget
{
Q_OBJECT
    SeaView* m_sea;
    StatsWidget* m_stats_widgets[2];
    Controller* m_controller;
    KScoreDialog* m_highscores;
    AudioPlayer* m_player;
    ChatWidget* m_chat;
    SimpleMenu* m_menu;
    QStatusBar* m_status_bar;
    
    void endGame();
public:
    PlayField(QWidget* parent, QStatusBar*);
    ~PlayField();
public slots:
    void highscores();
    void gameOver(Sea::Player winner);
    void setupController();
    
    void newGame();
    void restart();
signals:
    void gameFinished();
};

#endif // PLAYFIELD_H

