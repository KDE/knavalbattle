/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef COORD_H
#define COORD_H

#include <kdebug.h>

class Coord
{
public:
    int x;
    int y;
    
    Coord(int x, int y);
    Coord() { }
    
    Coord operator+(const Coord& other) const;
    Coord operator-(const Coord& other) const;
    Coord operator*(int n) const;
    Coord& operator+=(const Coord& other);
    bool operator==(const Coord& other) const;
    bool operator!=(const Coord& other) const;
    bool valid() const;
    
    static Coord invalid();
};

kdbgstream& operator<<(kdbgstream& os, const Coord& c);
uint qHash(const Coord& c);

#endif // COORD_H

