/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "animator.h"
#include "animation.h"

Animator* Animator::m_instance = 0;

Animator::Animator()
{
    m_group = new AnimationGroup;
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
}

Animator::~Animator()
{
    delete m_group;
}

void Animator::add(Animation* a)
{
    m_group->add(a);
    start();
}

void Animator::start()
{
    if (!m_timer.isActive()) {
        m_timer.start(0);
        m_time.restart();
        m_group->start(0);
    }
}

void Animator::stop()
{
    m_group->stop();
    m_timer.stop();
}

void Animator::restart()
{
    stop();
    start();
}

void Animator::tick()
{
    if (m_group->step(m_time.elapsed())) {
        stop();
    }
}

Animator* Animator::instance()
{
    if (!m_instance) {
        m_instance = new Animator;
    }
    return m_instance;
}

#include "animator.moc"


