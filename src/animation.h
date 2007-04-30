/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef ANIMATION_H
#define ANIMATION_H

#include <QList>
#include <QObject>
#include <QPoint>

class KGameCanvasItem;

class Animation : public QObject
{
Q_OBJECT
public:
    virtual ~Animation() { }
    virtual void start(int t) = 0;
    virtual bool step(int t) = 0;
};

class AnimationGroup : public Animation
{
Q_OBJECT
    typedef QList<Animation*> Animations;
    Animations m_animations;
    int m_running;
public:
    AnimationGroup();
    virtual ~AnimationGroup();
    virtual void start(int t);
    virtual void stop();
    virtual bool step(int t);
    
    void add(Animation*);
};

class FadeAnimation : public Animation
{
Q_OBJECT
    KGameCanvasItem* m_sprite;
    int m_from;
    int m_to;
    int m_time;
    int m_start;
public:
    FadeAnimation(KGameCanvasItem* sprite, int from, int to, int time);
    virtual void start(int t);
    virtual bool step(int t);
};

class MovementAnimation : public Animation
{
Q_OBJECT
    KGameCanvasItem* m_sprite;
    QPoint m_from;
    QPoint m_to;
    int m_time;
    int m_start;
public:
    MovementAnimation(KGameCanvasItem* sprite, const QPoint& from,
                      const QPoint& to, int time);
    virtual void start(int t);
    virtual bool step(int t);
};

#endif // ANIMATION_H

