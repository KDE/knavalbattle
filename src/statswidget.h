/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef STATSWIDGET_H
#define STATSWIDGET_H

#include <QWidget>

class Stats;
class QLabel;
class KGameLCD;
class Entity;

class StatsWidget : public QWidget
{
Q_OBJECT
    Stats* m_stats;
    
    QLabel* m_player_icon;
    QLabel* m_player_name;
    
    KGameLCD* m_shots;
    KGameLCD* m_hits;
    KGameLCD* m_misses;
    
    bool m_editable;
private slots:
    void updateHits();
    void updateMisses();
    void updateNick(int, const QString& nick);
public:
    StatsWidget(QWidget* parent);
    
    void setStats(Entity* stats);
    const Stats* stats() const { return m_stats; }
    void setEditable(bool value);
    bool editable() const;
};

#endif // STATSWIDGET_H

