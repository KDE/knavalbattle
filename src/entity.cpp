/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "entity.h"


Entity::Entity(Sea::Player player)
: m_player(player)
, m_level(COMPAT_KBS4)
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

#include "entity.moc"


