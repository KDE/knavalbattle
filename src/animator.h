/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <QTimer>
#include <QElapsedTimer>

class AnimationGroup;
class Animation;

class Animator : public QObject
{
Q_OBJECT
    AnimationGroup* m_group;
    QTimer m_timer;
    QElapsedTimer m_time;
    
    static Animator* m_instance;
    Animator();
public:
    static Animator* instance();
    
    ~Animator() override;
    void add(Animation*);
    
    void start();
    void stop();
    void restart();
private Q_SLOTS:
    void tick();
};

#endif // ANIMATOR_H

