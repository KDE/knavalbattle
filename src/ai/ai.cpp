/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "ai.h"

AI::AI(Sea::Player player, Sea* sea)
: m_player(player)
, m_sea(sea)
{
}

Coord AI::desperateMove() const
{
    Sea::Player opp = Sea::opponent(m_player);
    for (int i = 0; i < m_sea->size().x; i++)
    for (int j = 0; j < m_sea->size().y; j++) {
        if (m_sea->at(opp, Coord(i,j)).free()) {
            return Coord(i,j);
        }
    }
    return Coord::invalid();
}

