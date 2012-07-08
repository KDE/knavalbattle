/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef KBATTLESHIP_SPRITEFACTORY_H
#define KBATTLESHIP_SPRITEFACTORY_H

#include <QString>

class KBSRenderer;
class Sprite;
class Ship;

class SpriteFactory
{
    KBSRenderer* m_renderer;
    
    Sprite* createSimpleSprite(const QString& name);
public:
    SpriteFactory(KBSRenderer* renderer);
    
    Sprite* createShip(Ship* ship);
    Sprite* createHit();
    Sprite* createMiss();
    Sprite* createImpact();
};

#endif // KBATTLESHIP_SPRITEFACTORY_H
