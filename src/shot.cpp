/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "shot.h"
#include "controller.h"

Shot::Shot(Controller* parent, Sea::Player player, const Coord& pos)
: m_parent(parent)
, m_player(player)
, m_pos(pos)
{
}
    
void Shot::execute(const HitInfo& info) {
    m_parent->finalizeShot(m_player, m_pos, info);
}



