#ifndef SPRITEBUILDER_H
#define SPRITEBUILDER_H

#include <QString>

class KBSRenderer;
class KGameCanvasAbstract;
class Sprite;
class Ship;

class SpriteFactory
{
    KGameCanvasAbstract* m_canvas;
    KBSRenderer* m_renderer;
    
    Sprite* createSimpleSprite(const QString& name);
public:
    SpriteFactory(KGameCanvasAbstract* canvas, KBSRenderer* renderer);
    
    Sprite* createShip(Ship* ship);
    Sprite* createHit();
    Sprite* createMiss();
};

#endif // SPRITEBUILDER_H

