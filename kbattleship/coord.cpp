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

kdbgstream& operator<<(kdbgstream& os, const Coord& c)
{
    os << "(" << c.x << "," << c.y << ")";
    return os;
}

