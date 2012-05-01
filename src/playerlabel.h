/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef PLAYERLABEL_H
#define PLAYERLABEL_H

#define USE_UNSTABLE_LIBKDEGAMESPRIVATE_API
#include <libkdegamesprivate/kgamecanvas.h>

class PlayerLabel : public KGameCanvasGroup
{
public:
    static const int HEIGHT = 32;
    static const int MARGIN = 10;
    
private:
    KGameCanvasPixmap* m_icon;
    KGameCanvasText* m_name;
    
public:
    PlayerLabel(const QPixmap& icon, const QString& text, KGameCanvasAbstract* parent);

    void update();
    void setData(const QPixmap& icon, const QString& text);
    void setText(const QString& text);
    int height() const;
};

#endif // PLAYERLABEL_H

