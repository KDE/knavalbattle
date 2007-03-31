/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <kgamecanvas.h>
#include <QMultiHash>
#include "coord.h"
#include "spritefactory.h"
#include "grid.h"

class KBSRenderer;
class Sprite;
class Ship;
class Animator;

class BattleFieldView : public KGameCanvasGroup
{
    static const int PREVIEW_OPACITY = 120;

    KGameCanvasPixmap* m_background;
    KBSRenderer* m_renderer;
    Animator* m_animator;
    SpriteFactory m_factory;
    QString m_bgID;
    int m_gridSize;
    
    struct Preview {
        Coord pos;
        Ship* ship;
        Sprite* sprite;
        
        Preview()
        : pos(-1, -1)
        , ship(0)
        , sprite(0)
        {
        }
    } m_preview;
    
    typedef QMultiHash<Coord, Sprite*> Sprites;
    Sprites m_sprites;
    void addSprite(const Coord& c, Sprite* ship);
public:
    BattleFieldView(KGameCanvasAbstract* parent, KBSRenderer* renderer, 
                    Animator* animator, const QString& bgID, int gridSize);
    QSize size() const;
    
    void update();
    
    void setPreview(const QPoint& pos, Ship* ship);
    void cancelPreview();
    void add(const Coord& c, Ship* ship);
    void hit(const Coord& c);
    void miss(const Coord& c);
    void clear();
};

#endif // BATTLEFIELD_H

