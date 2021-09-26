/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SHOT_H
#define SHOT_H

#include "sea.h"

class Controller;

class Shot
{
    Controller* m_parent;
    Sea::Player m_player;
    Coord m_pos;
public:
    Shot(Controller* parent, Sea::Player player, const Coord& pos);
    void execute(const HitInfo& info);
    
    Sea::Player player() const { return m_player; }
    const Coord& pos() const { return m_pos; }
};


#endif // SHOT_H
