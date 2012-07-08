/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef SPRITE_H
#define SPRITE_H

#include <KGameRenderedItem>

#include "coord.h"

class KBSRenderer;

class Sprite : public KGameRenderedItem
{
    Coord m_scale;
    QString m_name;
    bool m_rotated;
public:
    Sprite(KBSRenderer* renderer, const Coord& scale, const QString& name, bool rotated);
    ~Sprite();

    void refresh(KBSRenderer* renderer);
};

#endif // SPRITE_H

