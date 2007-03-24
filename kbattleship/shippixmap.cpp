#include <sstream>

#include "shippixmap.h"
#include "kbsrenderer.h"

ShipPixmap::ShipPixmap(KGameCanvasAbstract* parent, Ship* ship, KBSRenderer* renderer)
: KGameCanvasPixmap(parent)
, m_ship(ship)
{
    update(renderer);
}


void ShipPixmap::update(KBSRenderer* renderer)
{
    int x;
    int y;
    if (m_ship->direction() == Ship::LEFT_TO_RIGHT) {
        x = m_ship->size();
        y = 1;
    }
    else {
        x = 1;
        y = m_ship->size();
    }
    std::ostringstream os;
    os << "ship" << m_ship->size() << "-view";
    setPixmap(renderer->render(os.str().c_str(), x, y));
}


