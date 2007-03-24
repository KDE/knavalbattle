#ifndef GRID_H
#define GRID_H

#include <qdebug.h>
#include "coord.h"

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
T& Grid<T>::operator[](const Coord& p)
{
    int n = convert(p);
    Q_ASSERT(n >= 0 && n < m_size.x * m_size.y);
    return m_grid[n];
}

template <typename T>
const T& Grid<T>::operator[](const Coord& p) const
{
    int n = convert(p);
    Q_ASSERT(n >= 0 && n < m_size.x * m_size.y);
    return m_grid[n];
}


#endif // GRID_H

