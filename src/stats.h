/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef STATS_H
#define STATS_H

#include <QObject>

class HitInfo;

class Stats : public QObject
{
Q_OBJECT
    int m_hits;
    int m_misses;
public:
    Stats();
    
    inline int hits() const { return m_hits; }
    void addHit();
    inline int misses() const { return m_misses; }
    void addMiss();
    inline int shots() const { return hits() + misses(); }
    
    void addInfo(const HitInfo& info);
    
    int score() const;
    
signals:
    void hitsChanged();
    void missesChanged();
};

#endif // STATS_H

