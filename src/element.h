/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
    Element(Type = WATER);
    explicit Element(Ship* parent);
    inline Ship* parent() { return m_parent; }
    inline const Ship* parent() const { return m_parent; }
    inline void setParent(Ship* ship) { m_parent = ship; }
    inline Type type() const { return m_type; }
    inline void setType(Type type) { m_type = type; }
    
    bool free() const;
    bool water() const;
    HitInfo::Type hit();
};

#endif // ELEMENT_H

