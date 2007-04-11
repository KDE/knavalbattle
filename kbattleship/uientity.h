/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef UIENTITY_H
#define UIENTITY_H

#include "entity.h"

class SeaView;

class UIEntity : public Entity
{
Q_OBJECT
protected:
    Sea* m_sea;
    SeaView* m_view;
    bool m_started;
    
    void drawShoot(Sea::Player player, const Coord& c, const HitInfo& info);
public:
    UIEntity(Sea::Player player, Sea*, SeaView* view);
    virtual ~UIEntity();
    
    virtual void notify(Sea::Player player, const Coord& c, const HitInfo& info);
    virtual void start();
    virtual HitInfo hit(Sea::Player, const Coord&) { return HitInfo::INVALID; }
    
    virtual void registerHit(Sea::Player, const Coord&) { }
    virtual void registerMiss(Sea::Player, const Coord&) { }
    
    inline bool started() const { return m_started; }
};

#endif // UIENTITY_H
