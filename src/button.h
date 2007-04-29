/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef BUTTON_H
#define BUTTON_H

#include "welcomescreen.h"
#include <QIcon>
#include <QFont>
#include <QSize>

class Button : public QObject, public KGameCanvasPixmap
{
Q_OBJECT
    QIcon m_icon;
    QFont m_font;
    QString m_text;
    QSize m_size;
    
    bool m_down;
    bool m_hover;
    
    virtual void repaint();
public:
    Button(WelcomeScreen* parent, const QIcon& icon, 
           const QFont& font, const QString& text);
           
    QSize size() const;
    
    void onMousePress(const QPoint& p);
    void onMouseRelease(const QPoint& p);
    void onMouseMove(const QPoint& p);
    void onMouseLeave();
    void onClicked();
signals:
    void clicked();
};

#endif // BUTTON_H
