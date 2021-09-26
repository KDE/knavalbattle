/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef STATS_H
#define STATS_H

#include <QObject>

struct HitInfo;

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
    
Q_SIGNALS:
    void hitsChanged();
    void missesChanged();
};

#endif // STATS_H

