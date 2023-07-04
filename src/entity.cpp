/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "entity.h"


Entity::Entity(Sea::Player player, SeaView* seaview, const BattleShipsConfiguration* battleShipsConfiguration)
: m_player(player)
, m_seaview(seaview)
, m_level(COMPAT_KBS4)
, m_battleShipsConfiguration(battleShipsConfiguration)
, m_restarted(false)
{
}

Entity::~Entity()
{
}

void Entity::setNick(const QString& nickname)
{
    m_nick = nickname;
}

void Entity::setCompatibilityLevel(int level)
{
    
    m_level = static_cast<CompatibilityLevel>(level);
}

Stats* Entity::stats()
{
    return &m_stats;
}

#include "moc_entity.cpp"
