#ifndef ANIMATION_H
#define ANIMATION_H

class KGameCanvasItem;

class Animation
{
public:
    virtual ~Animation() { }
    virtual void start(int t) = 0;
    virtual bool step(int t) = 0;
};

class AnimationGroup : public Animation
{
    typedef QList<Animation*> Animations;
    Animations m_animations;
    int m_running;
public:
    AnimationGroup();
    virtual void start(int t);
    virtual bool step(int t);
    
    void add(Animation*);
};

class FadeAnimation : public Animation
{
    KGameCanvasItem* m_sprite;
    int m_from;
    int m_to;
    int m_time;
    int m_start;
public:
    FadeAnimation(KGameCanvasItem* sprite, int from, int to, int m_time);
    virtual void start(int t);
    virtual bool step(int t);
};

#endif // ANIMATION_H

