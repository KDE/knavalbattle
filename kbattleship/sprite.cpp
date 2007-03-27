/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include <kdebug.h>

#include "sprite.h"
#include "kbsrenderer.h"

Sprite::Sprite(KGameCanvasAbstract* parent, KBSRenderer* renderer, 
            const Coord& scale, const QString& name, bool rotated)
: KGameCanvasPixmap(parent)
, m_scale(scale)
, m_name(name)
, m_rotated(rotated)
{
    update(renderer);
}

Sprite::~Sprite()
{
}

void Sprite::update(KBSRenderer* renderer)
{
    setPixmap(renderer->render(m_name, m_rotated, m_scale.x, m_scale.y));
}
