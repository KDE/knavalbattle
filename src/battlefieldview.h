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
#include <QTime>
#include <QTimer>
#include "coord.h"
#include "spritefactory.h"
#include "grid.h"

class KBSRenderer;
class Sprite;
class Ship;
class WelcomeScreen;

class BattleFieldView : public KGameCanvasGroup
{
    static const int PREVIEW_OPACITY = 120;

    KGameCanvasPixmap* m_background;
    KGameCanvasPixmap* m_background_lower;
    WelcomeScreen* m_screen;
    KBSRenderer* m_renderer;
    SpriteFactory m_factory;
    QString m_bgID;
    int m_gridSize;
    Sprite* m_impact;
    Sprite* m_last_hit;
    
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
    BattleFieldView(KGameCanvasWidget* parent, KBSRenderer* renderer, const QString& bgID, int gridSize);
    QSize size() const;
    
    void update();
    
    void setPreview(const QPoint& pos, Ship* ship);
    void cancelPreview();
    void add(const Coord& c, Ship* ship);
    void hit(const Coord& c);
    void miss(const Coord& c);
    void removeImpact();
    void sink(const Coord& c, Ship* ship);
    void clear();
    
    
    void onMousePress(const QPoint& p);
    void onMouseRelease(const QPoint& p);
    void onMouseMove(const QPoint& p);
    void onMouseLeave();
    
    WelcomeScreen* screen() const;
};

#endif // BATTLEFIELD_H

