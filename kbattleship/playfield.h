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

class PlayField : public QWidget
{
Q_OBJECT
    SeaView* m_sea;
    Controller* m_controller;
    KScoreDialog* m_highscores;
    int m_human_player;
    QString m_nick;
    
    void setup(Controller*);
public:
    PlayField(QWidget* parent);
public slots:
    void newGame();
    void newSimulation();
    void highscores();
    void gameOver(Sea::Player winner);
};

#endif // PLAYFIELD_H

