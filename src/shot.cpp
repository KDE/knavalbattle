/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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



