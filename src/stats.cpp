/*
  Copyright (c) 2000-2001 Nikolas Zimmermann <wildfox@kde.org>
            (c) 2000-2001 Daniel Molkentin <molkentin@kde.org>
            (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "stats.h"
#include "hitinfo.h"

Stats::Stats()
: m_hits(0)
, m_misses(0)
{
}

int Stats::score() const
{
    // Balancing factors
    // a = shot-balance
    // b = water-balance
    double a = 3;
    double b = 0.5;
    double score = (a * hits() - b * misses()) / (shots() + misses()) * 1000;
    int res = static_cast<int>(score);
    if (res <= 0) {
        res = 1;
    }
    return res;
}

void Stats::addHit()
{
    m_hits++;
    emit hitsChanged();
}

void Stats::addMiss()
{
    m_misses++;
    emit missesChanged();
}

void Stats::addInfo(const HitInfo& info)
{
    switch (info.type)
    {
    case HitInfo::HIT:
        addHit();
        break;
    case HitInfo::MISS:
        addMiss();
        break;
    default:
        break;
    }
}




