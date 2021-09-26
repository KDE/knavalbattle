/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "animator.h"
#include "animation.h"

Animator* Animator::m_instance = nullptr;

Animator::Animator()
{
    m_group = new AnimationGroup;
    connect(&m_timer, &QTimer::timeout, this, &Animator::tick);
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




