/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "spritefactory.h"

#include <QString>

#include "ship.h"
#include "sprite.h"

SpriteFactory::SpriteFactory(KBSRenderer* renderer)
: m_renderer(renderer)
{
}

Sprite* SpriteFactory::createShip(Ship* ship)
{
    int x;
    int y;
    if (ship->direction() == Ship::LEFT_TO_RIGHT) {
        x = ship->size();
        y = 1;
    }
    else {
        x = 1;
        y = ship->size();
    }

    const QString name = QLatin1String("ship") + QString::number(ship->size()) + QLatin1String("-view");

    return new Sprite(m_renderer, Coord(x, y), name, ship->direction() == Ship::TOP_DOWN);
}

Sprite* SpriteFactory::createSimpleSprite(const QString& name)
{
    return new Sprite(m_renderer, Coord(1, 1), name, false);
}

Sprite* SpriteFactory::createHit()
{
    return createSimpleSprite(QStringLiteral("hit"));
}

Sprite* SpriteFactory::createMiss()
{
    return createSimpleSprite(QStringLiteral("water"));
}

Sprite* SpriteFactory::createImpact()
{
    return createSimpleSprite(QStringLiteral("water-impact"));
}


