/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef STATS_H
#define STATS_H

class Stats
{
    int m_hits;
    int m_misses;
public:
    Stats();
    
    inline int hits() const { return m_hits; }
    inline void addHit() { m_hits++; }
    inline int misses() const { return m_misses; }
    inline void addMiss() { m_misses++; }
    inline int shots() const { return hits() + misses(); }
    
    int score() const;
};

#endif // STATS_H

