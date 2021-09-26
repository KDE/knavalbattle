/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef COORD_H
#define COORD_H

#include <QMetaType>
#include "knavalbattle_debug.h"

class Coord
{
public:
    int x;
    int y;
    
    Coord(int x, int y);
    Coord():x(0),y(0) { }
    
    Coord operator+(const Coord& other) const;
    Coord operator-(const Coord& other) const;
    Coord operator*(int n) const;
    Coord& operator+=(const Coord& other);
    Coord& operator-=(const Coord& other);
    bool operator==(const Coord& other) const;
    bool operator!=(const Coord& other) const;
    bool valid() const;
    
    static Coord invalid();
};

QDebug& operator<<(QDebug& os, const Coord& c);
uint qHash(const Coord& c);
Q_DECLARE_METATYPE(Coord)

#endif // COORD_H

