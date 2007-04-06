/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "coord.h"

Coord::Coord(int x, int y)
: x(x)
, y(y)
{
}

Coord Coord::operator+(const Coord& other) const
{
    return Coord(x + other.x, y + other.y);
}

Coord Coord::operator-(const Coord& other) const
{
    return Coord(x - other.x, y - other.y);
}

Coord Coord::operator*(int n) const
{
    return Coord(x * n, y * n);
}

Coord& Coord::operator+=(const Coord& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

bool Coord::operator==(const Coord& other) const
{
    return x == other.x && y == other.y;
}

bool Coord::valid() const
{
    return x != -1 && y != -1;
}

Coord Coord::invalid()
{
    return Coord(-1, -1);
}

kdbgstream& operator<<(kdbgstream& os, const Coord& c)
{
    os << "(" << c.x << "," << c.y << ")";
    return os;
}

uint qHash(const Coord& c)
{
    return (uint) c.x * 100 + c.y;
}
