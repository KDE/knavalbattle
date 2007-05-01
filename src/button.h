/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef BUTTON_H
#define BUTTON_H

#include <QIcon>
#include <QFont>
#include <QSize>
#include <QPointer>
#include "welcomescreen.h"
#include "animation.h"

class ButtonAnimation;

class Button : public QObject, public KGameCanvasPixmap
{
Q_OBJECT
    enum {
        BRIGHTNESS_NORMAL = 0,
        BRIGHTNESS_HOVER = 120,
        BRIGHTNESS_DOWN = 180
    };
    QIcon m_icon;
    QFont m_font;
    QString m_text;
    QSize m_size;
    
    bool m_down;
    bool m_hover;
    double m_brightness;
    QPointer<ButtonAnimation> m_animation;
    
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
    
    void setBrightness(double value);
    double brightness() const;
    
    KGameCanvasPixmap* extractIcon();
signals:
    void clicked();
};

class ButtonAnimation : public Animation
{
Q_OBJECT
public:
    Button* m_button;
    int m_brightness;
    static double m_speed;
    double m_current;
    int m_last;
public:
    ButtonAnimation(Button* button, int brightness);
    ~ButtonAnimation();
    virtual void start(int t);
    virtual bool step(int t);
    virtual void abort();
    
    void setBrightness(int value);
};

#endif // BUTTON_H
