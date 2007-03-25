#ifndef Ship_H
#define Ship_H

#include "coord.h"
#include "element.h"

class Ship
{
public:
    enum Direction
    {
        TOP_DOWN,
        LEFT_TO_RIGHT
    };
private:
    unsigned int m_size;
    Direction m_direction;
    int m_life;
public:
    Ship(unsigned int size, Direction direction);

    void changeDirection();
    Coord increment() const;
    static Coord increment(Direction);
    inline unsigned int size() const { return m_size; }
    inline Direction direction() const { return m_direction; }
    void decLife();
    bool alive() const;
};

#endif // Ship_H
