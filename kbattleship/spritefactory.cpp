#include <sstream>

#include "spritefactory.h"
#include "ship.h"
#include "sprite.h"

SpriteFactory::SpriteFactory(KGameCanvasAbstract* canvas, KBSRenderer* renderer)
: m_canvas(canvas)
, m_renderer(renderer)
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
    std::ostringstream os;
    os << "ship" << ship->size() << "-view";
    
    return new Sprite(m_canvas, m_renderer, Coord(x, y), os.str().c_str());
}

Sprite* SpriteFactory::createSimpleSprite(const QString& name)
{
    return new Sprite(m_canvas, m_renderer, Coord(1, 1), name);
}

Sprite* SpriteFactory::createHit()
{
    return createSimpleSprite("hit");
}

Sprite* SpriteFactory::createMiss()
{
    return createSimpleSprite("water");
}


