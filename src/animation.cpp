/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "animation.h"
#include "knavalbattle_debug.h"

Animation::~Animation()
{
    Q_EMIT done();
}

AnimationGroup::AnimationGroup()
: m_running(-1)
{
}

AnimationGroup::~AnimationGroup()
{
    stop();
}

void AnimationGroup::start(int t)
{
    m_running = t;
    for (Animation* a : std::as_const(m_animations)) {
        a->start(t);
    }
}

void AnimationGroup::stop()
{
    m_running = -1;
    qDeleteAll(m_animations);
    m_animations.clear();
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

FadeAnimation::FadeAnimation(QGraphicsItem* sprite, int from, qreal to, int time)
: m_sprite(sprite)
, m_from(from)
, m_to(to)
, m_time(time)
, m_start(0)
{
}

void FadeAnimation::start(int t)
{
    m_start = t;
    m_sprite->setOpacity(m_from);
}

bool FadeAnimation::step(int t)
{
    if (t >= m_time + m_start) {
        m_sprite->setOpacity(m_to);
        return true;
    }
    else {
        if (m_to > m_from) {
            m_sprite->setOpacity(m_from + t * (m_to / m_time));
        }
        else {
            m_sprite->setOpacity(m_from - t * (m_to / m_time));
        }
        return false;
    }
}


MovementAnimation::MovementAnimation(QGraphicsItem* sprite, const QPointF& from, 
                                     const QPointF& to, int time)
: m_sprite(sprite)
, m_from(from)
, m_to(to)
, m_time(time)
, m_start(0)
{
}

void MovementAnimation::start(int t)
{
    m_start = t;
    m_sprite->setPos(m_from);
}

bool MovementAnimation::step(int t)
{
    if (t >= m_start + m_time) {
        m_sprite->setPos(m_to);
        return true;
    }
    else {
        QPointF pos = m_from + (m_to - m_from) * (t - m_start) / m_time;
        m_sprite->setPos(pos);
        return false;
    }
}

#include "moc_animation.cpp"
