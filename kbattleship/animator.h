#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <QTimer>
#include <QTime>
#include "animation.h"

class Animator : public QObject
{
Q_OBJECT
    AnimationGroup m_group;
    QTimer m_timer;
    QTime m_time;
public:
    Animator();
    void add(Animation*);
    
    void start();
private slots:
    void tick();
};

#endif // ANIMATOR_H

