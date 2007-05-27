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
#include <QHash>
#include "grid.h"

class Button;

class WelcomeScreen : public QObject, public KGameCanvasGroup
{
Q_OBJECT
    typedef QHash<Coord, Button*> Buttons;
    Buttons m_buttons;
    QFont m_font;
    QSize m_size;
    
    KGameCanvasRectangle* m_background;
    
    Button* m_clicked;
    Button* m_hover;
    bool m_active;
public:
	WelcomeScreen(KGameCanvasAbstract* parent, const QFont& font);
    
    void resize(const QSize&);
    
    Button* addButton(int x, int y, const QIcon& icon, const QString& text);
    void removeButton(int x, int y);
    void moveButton(int x1, int y1, int x2, int y2);
    void clearButtons();
    
    void onMousePress(const QPoint& p);
    void onMouseRelease(const QPoint& p);
    void onMouseMove(const QPoint& p);
    void onMouseLeave();
    
    bool active() const { return m_active; }
    
public slots:
    void hide();
    void fadeOut();
    void update();
    
signals:
    void clicked(Button*);
    void hidden();
};

#endif // WELCOMESCREEN_H
