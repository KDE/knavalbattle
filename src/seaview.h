/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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
    ~SeaView() override;
    
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
public Q_SLOTS:
    void refresh();
};

#endif // SEA_H

