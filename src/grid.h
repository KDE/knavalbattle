/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef GRID_H
#define GRID_H

#include <QDebug>

#include "coord.h"

#define FOREACH_SQUARE(p, grid) \
    for (Coord p(0,0); p.x < grid.width(); p.x++) \
    for (p.y = 0; p.y < grid.height(); p.y++)

template <typename T>
class Grid
{
    Coord m_size;
    T* m_grid;
    
    int convert(const Coord& p) const;
public:
    Grid(const Coord& size);
    ~Grid();
    
    T& operator[](const Coord& p);
    const T& operator[](const Coord& p) const;
    
    bool valid(const Coord& p) const;
    inline int width() const { return m_size.x; }
    inline int height() const { return m_size.y; }
};

// Implementation

template <typename T>
Grid<T>::Grid(const Coord& size)
: m_size(size)
{
    m_grid = new T[m_size.x * m_size.y];
}

template <typename T>
Grid<T>::~Grid()
{
    delete[] m_grid;
}

template <typename T>
int Grid<T>::convert(const Coord& p) const
{
    return p.x + p.y * m_size.x;
}

template <typename T>
bool Grid<T>::valid(const Coord& p) const
{
    return p.x >= 0 && p.x < width() &&
            p.y >= 0 && p.y < height();
}

template <typename T>
T& Grid<T>::operator[](const Coord& p)
{
    Q_ASSERT(valid(p));
    return m_grid[convert(p)];
}

template <typename T>
const T& Grid<T>::operator[](const Coord& p) const
{
    Q_ASSERT(valid(p));
    return m_grid[convert(p)];
}


#endif // GRID_H

