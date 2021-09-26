/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ANIMATION_H
#define ANIMATION_H

#include <QList>
#include <QObject>
#include <QPoint>
#include <QGraphicsItem>


class Animation : public QObject
{
Q_OBJECT
public:
    ~Animation() override;
    virtual void start(int t) = 0;
    virtual bool step(int t) = 0;
    
Q_SIGNALS:
    void done();
};

class AnimationGroup : public Animation
{
Q_OBJECT
    typedef QList<Animation*> Animations;
    Animations m_animations;
    int m_running;
public:
    AnimationGroup();
    ~AnimationGroup() override;
    void start(int t) override;
    bool step(int t) override;
    virtual void stop();
    
    void add(Animation*);
};

class FadeAnimation : public Animation
{
Q_OBJECT
    QGraphicsItem* m_sprite;
    int m_from;
    qreal m_to;
    int m_time;
    int m_start;
public:
    FadeAnimation(QGraphicsItem* sprite, int from, qreal to, int time);
    void start(int t) override;
    bool step(int t) override;
};

class MovementAnimation : public Animation
{
Q_OBJECT
    QGraphicsItem* m_sprite;
    QPointF m_from;
    QPointF m_to;
    int m_time;
    int m_start;
public:
    MovementAnimation(QGraphicsItem* sprite, const QPointF& from,
                      const QPointF& to, int time);
    void start(int t) override;
    bool step(int t) override;
};

#endif // ANIMATION_H

