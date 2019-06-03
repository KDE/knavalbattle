/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef SEA_H
#define SEA_H

#include <QWidget>

#include "sea.h"
#include "animator.h"

class BattleFieldView;
class KBSRenderer;
class Delegate;
class WelcomeScreen;
class PlayerLabel;
class StatsWidget;
class Stats;

class SeaView : public QWidget 
{
Q_OBJECT
    static const unsigned int GRID_SIZE = 10;
    static const unsigned int LABEL_SPACING = 4;
    static const unsigned int MARGIN = 5;
    static const unsigned int GAP = 2;
    
    WelcomeScreen* m_screen;
    BattleFieldView* m_fields[2];
    PlayerLabel* m_labels[2];
    StatsWidget* m_stats[2];
    
    KBSRenderer* m_renderer;
    Delegate* m_delegate;
    BattleFieldView* otherField(BattleFieldView* field);
    QPoint m_last_preview;
    int m_last_f;
    
    bool setPreview(Sea::Player player, const QPoint& pos);
    bool updatePreview(const QPoint& pos);
    int fieldAt(const QPoint& p);
public:
    explicit SeaView(QWidget* parent);
    ~SeaView();
    
    void setDelegate(Delegate* c);
    void cancelPreview();
    void add(Sea::Player p, Ship* ship);
    void hit(Sea::Player p, const Coord& c);
    void miss(Sea::Player p, const Coord& c);
    void sink(Sea::Player p, Ship* ship);
    void clear();
    void setStatus(Sea::Status status);
    
    void setStats(Sea::Player p, const QString& icon, 
                  const QString& text, Stats* stats);
    void setNick(Sea::Player p, const QString& nick);

    WelcomeScreen* globalScreen() const;
    WelcomeScreen* screen(Sea::Player player) const;

    void toggleLeftGrid(bool show);
    void toggleRightGrid(bool show);
protected:
    void resizeEvent(QResizeEvent*) override;
    
    QSize tileSize() const;
public slots:
    void refresh();
};

#endif // SEA_H

