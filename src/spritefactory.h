/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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
    explicit SpriteFactory(KBSRenderer* renderer);
    
    Sprite* createShip(Ship* ship);
    Sprite* createHit();
    Sprite* createMiss();
    Sprite* createImpact();
};

#endif // KBATTLESHIP_SPRITEFACTORY_H
