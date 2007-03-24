#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <kgamecanvas.h>
#include "coord.h"

class KBSRenderer;
class ShipPixmap;
class Ship;

class BattleFieldView : public KGameCanvasGroup
{
    KGameCanvasPixmap* m_background;
    KBSRenderer* m_renderer;
    int m_gridSize;
    ShipPixmap* m_preview;
    
    void addSprite(const Coord&, const QString& name);
public:
    BattleFieldView(KGameCanvasAbstract* parent, KBSRenderer* renderer, int gridSize);
    QSize size() const;
    
    void update();
    
    void setPreview(const QPoint& pos, Ship* ship);
    void cancelPreview();
    void add(const Coord& c, Ship* ship);
    void hit(const Coord& c);
    void miss(const Coord& c);
};

#endif // BATTLEFIELD_H

