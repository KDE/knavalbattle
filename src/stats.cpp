/*
    SPDX-FileCopyrightText: 2000-2001 Nikolas Zimmermann <wildfox@kde.org>
    SPDX-FileCopyrightText: 2000-2001 Daniel Molkentin <molkentin@kde.org>
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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
    Q_EMIT hitsChanged();
}

void Stats::addMiss()
{
    m_misses++;
    Q_EMIT missesChanged();
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




