#ifndef COORD_H
#define COORD_H

#include <kdebug.h>

class Coord
{
public:
    int x;
    int y;
    
    Coord(int x, int y);
    
    Coord operator+(const Coord& other) const;
    Coord operator*(int n) const;
    Coord& operator+=(const Coord& other);
    bool operator==(const Coord& other) const;
};

kdbgstream& operator<<(kdbgstream& os, const Coord& c);
uint qHash(const Coord& c);

#endif // COORD_H

