/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "welcomescreen.h"

#include <kdebug.h>

WelcomeScreen::WelcomeScreen(KGameCanvasAbstract* parent, const QFont& font)
: KGameCanvasItem(parent)
, KWelcomeScreenOverlay(font)
{
}

void WelcomeScreen::paint(QPainter* p)
{
    p->save();
    p->translate(pos());
    drawScreen(*p);
    p->restore();
}

QRect WelcomeScreen::rect() const
{
    return QRect(pos(), m_size);
}

void WelcomeScreen::invalidateScreen(const QRect &)
{
    changed();
}

