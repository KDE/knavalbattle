/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

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

