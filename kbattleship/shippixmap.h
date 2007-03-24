#ifndef SHIPPIXMAP_H
#define SHIPPIXMAP_H

#include <kgamecanvas.h>

class KBSRenderer;
class Ship;

class ShipPixmap : public KGameCanvasPixmap
{
    Ship* m_ship;
public:
    ShipPixmap(KGameCanvasAbstract* parent, Ship* ship, KBSRenderer* renderer);
    
    void update(KBSRenderer* renderer);
};

#endif // SHIPPIXMAP_H

