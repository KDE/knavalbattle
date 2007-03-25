#include "animator.h"

Animator::Animator()
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
}

void Animator::add(Animation* a)
{
    m_group.add(a);
}

void Animator::start()
{
    m_timer.start(20);
    m_time.start();
    m_group.start(0);
}

void Animator::tick()
{
    m_group.step(m_time.elapsed());
}

#include "animator.moc"


