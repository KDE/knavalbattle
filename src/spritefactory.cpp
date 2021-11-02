/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "spritefactory.h"


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


