/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "sprite.h"

#include <kdebug.h>

#include "kbsrenderer.h"

Sprite::Sprite(KBSRenderer* renderer, const Coord& scale, const QString& name, bool rotated)
: KGameRenderedItem(renderer, name)
, m_scale(scale)
, m_name(name)
, m_rotated(rotated)
{
    refresh(renderer);
}

Sprite::~Sprite()
{
}

void Sprite::refresh(KBSRenderer* renderer)
{
    if (m_rotated)
    {
        setTransformOriginPoint(renderer->size().width() / 2,
                                renderer->size().height() / 2);
        setRotation(90);
       
        QSize renderSize(renderer->size().height() * m_scale.y,
                         renderer->size().width()  * m_scale.x);
        setRenderSize(renderSize);
    }
    else
    {
        QSize renderSize(renderer->size().width()  * m_scale.x,
                         renderer->size().height() * m_scale.y);
        setRenderSize(renderSize);   
    }
}
