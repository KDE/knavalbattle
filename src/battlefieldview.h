/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef BATTLEFIELDVIEW_H
#define BATTLEFIELDVIEW_H

#include <QMultiHash>
#include <QTime>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsLineItem>

#include <KGameRenderedItem>

#include "coord.h"
#include "spritefactory.h"
#include "grid.h"
#include "sea.h"

class KBSRenderer;
class Sprite;
class Ship;
class WelcomeScreen;
class Delegate;

class BattleFieldView : public QGraphicsView 
{
    static const qreal PREVIEW_OPACITY;

    KGameRenderedItem* m_background;
    KGameRenderedItem* m_background_lower;

    QGraphicsLineItem *hlines[11];
    QGraphicsLineItem *vlines[11];

    enum Planes {
        BACKGROUND = 1,
        FOREGROUND = 2
    };

    WelcomeScreen* m_screen;
    KBSRenderer* m_renderer;
    SpriteFactory m_factory;
    QString m_bgID;
    int m_gridSize;
    Sprite* m_impact;
    Sprite* m_last_hit;
    bool m_drawGrid;

    Delegate *m_delegate;
    Sea::Player m_player;
    Sea::Status m_status;

    struct Preview {
        Coord pos;
        Ship* ship;
        Sprite* sprite;
        
        Preview()
        : pos(-1, -1)
        , ship(nullptr)
        , sprite(nullptr)
        {
        }
    } m_preview;
    
    typedef QMultiHash<Coord, Sprite*> Sprites;
    Sprites m_sprites;

private:
    void addSprite(const Coord& c, Sprite* ship);
    void loadPreviewSprite(Ship * ship);

public:
    BattleFieldView(QWidget* parent, KBSRenderer* renderer, const QString& bgID, int gridSize);

    void toggleGrid(bool show);
    void refresh();
    
    void setPreview(const QPoint &pos);
    void cancelPreview();
    void add(Ship* ship);
    void hit(const Coord& c);
    void miss(const Coord& c);
    void removeImpact();
    void sink(Ship* ship);
    void clear();

    void setDelegate(Delegate *c);
    void setPlayer(Sea::Player player);

    WelcomeScreen* screen() const;
    inline void setStatus(Sea::Status status) { m_status = status; };
protected:
    void drawGrid();

    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void leaveEvent(QEvent *) override;
};

#endif // BATTLEFIELD_H

