/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <kgamecanvas.h>
#include "kwelcomescreen.h"

class WelcomeScreen : public KGameCanvasItem, public KWelcomeScreenOverlay
{
protected:
	virtual void invalidateScreen(const QRect &);
	virtual void buttonClicked(KWelcomeScreenOverlayButton* ) { }
public:
	WelcomeScreen(KGameCanvasAbstract* parent, const QFont& font);

	virtual QRect rect() const;
	virtual void paint(QPainter* p);
};

#endif // WELCOMESCREEN_H
