#ifndef ELEMENT_H
#define ELEMENT_H

#include "hitinfo.h"

class Ship;

class Element
{
public:
    enum Type
    {
        ALIVE,
        DEAD,
        MISS,
        BORDER,
        WATER
    };
private:
    Ship* m_parent;
    Type m_type;
public:
    Element();
    Element(Ship* parent);
    inline Ship* parent() { return m_parent; }
    inline const Ship* parent() const { return m_parent; }
    bool free() const;
    bool water() const;
    HitInfo::Type hit();
};

#endif // ELEMENT_H

