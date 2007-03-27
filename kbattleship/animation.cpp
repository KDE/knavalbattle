/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include <kgamecanvas.h>
#include <kdebug.h>
#include "animation.h"

AnimationGroup::AnimationGroup()
: m_running(-1)
{
}

void AnimationGroup::start(int t)
{
    m_running = t;
    foreach (Animation* a, m_animations) {
        a->start(t);
    }
}

bool AnimationGroup::step(int t)
{
    m_running = t;
    for (Animations::iterator i = m_animations.begin();
            i != m_animations.end(); ) {
        if ((*i)->step(t)) {
            delete *i;
            i = m_animations.erase(i);
        }
        else {
            ++i;
        }
    }
    return m_animations.empty();
}

void AnimationGroup::add(Animation* a)
{
    m_animations.append(a);
    if (m_running != -1) {
        a->start(m_running);
    }
}

FadeAnimation::FadeAnimation(KGameCanvasItem* sprite, int from, int to, int time)
: m_sprite(sprite)
, m_from(from)
, m_to(to)
, m_time(time)
{
}

void FadeAnimation::start(int t)
{
    m_start = t;
    m_sprite->setOpacity(m_from);
}

bool FadeAnimation::step(int t)
{
    int opacity = int(m_from + (m_to - m_from) * (t - m_start) / (double)m_time);
    m_sprite->setOpacity(opacity);
    return opacity >= m_to;
}



