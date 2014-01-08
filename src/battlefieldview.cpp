/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "battlefieldview.h"

#include <QMouseEvent>
#include <QSizePolicy>
#include <kdebug.h>
#include <kicon.h>

#include "kbsrenderer.h"
#include "sprite.h"
#include "animator.h"
#include "animation.h"
#include "welcomescreen.h"
#include "button.h"
#include "delegate.h"
#include "sea.h"

BattleFieldView::BattleFieldView(QWidget* parent, KBSRenderer* renderer, const QString& bgID, int gridSize)
: QGraphicsView(parent)
, m_renderer(renderer)
, m_factory(renderer)
, m_bgID(bgID)
, m_gridSize(gridSize)
, m_impact(0)
, m_last_hit(0)
, m_drawGrid(true)
, m_delegate(0)
{
    m_background_lower = new KGameRenderedItem(m_renderer, bgID + "-layer1");
    m_background_lower->setOpacity(0.98);
    
    m_background = new KGameRenderedItem(m_renderer, bgID + "-layer2");
    m_background->setOpacity(0.98);
    
    m_screen = new WelcomeScreen(font());

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addItem(m_background_lower);
    scene->addItem(m_background);
    scene->addItem(m_screen);
    
    for (Sprites::iterator i = m_sprites.begin();
            i != m_sprites.end();
            ++i) {
        i.value() = 0;
    }

    for (int i = 0; i < 11; i++) {
        hlines[i] = new QGraphicsLineItem;
        vlines[i] = new QGraphicsLineItem;

        scene->addItem(hlines[i]);
        scene->addItem(vlines[i]);

        hlines[i]->stackBefore(m_background);
        vlines[i]->stackBefore(m_background);
    }

    setScene(scene);
    setMouseTracking(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void BattleFieldView::toggleGrid(bool show)
{
    if (m_drawGrid != show)
    {
        m_drawGrid = show;
        drawGrid();
    }
}

void BattleFieldView::drawGrid()
{
    if (m_drawGrid)
    {
        int spacing = m_renderer->size().width();
        int width   = spacing * m_gridSize;
        int height  = spacing * m_gridSize;

        for (int i = 0; i < 11; i++) {
            hlines[i]->show();
            hlines[i]->setLine(0, i * spacing, width, i * spacing);

            vlines[i]->show();
            vlines[i]->setLine(i * spacing, 0, i * spacing, height);
        }
    }
    else
    {
        for (int i = 0; i < 11; i++) {
            hlines[i]->hide();
            vlines[i]->hide();
        }
    }
}

void BattleFieldView::refresh()
{
    // Updates this widget.
    int x = pos().x();
    int y = pos().x();
    int w = m_renderer->size().width() * m_gridSize;
    int h = m_renderer->size().height() * m_gridSize;

    setSceneRect(0, 0, w, h);
    // Due the rounded nature of the view, it's necessary to set its
    // geometry to make sure it will show the entire grid.
    int fs = frameWidth();
    setGeometry(x, y, w + 2 * fs, h + 2 * fs); 

    // update welcome screen
    m_screen->setPos(0, 0);
    m_screen->resize(m_renderer->size() * m_gridSize);

    // Updates the backgrounds.
    m_background_lower->hide();
    m_background_lower->setRenderSize(m_renderer->size() * m_gridSize);
    m_background_lower->show();

    m_background->hide();
    m_background->setRenderSize(m_renderer->size() * m_gridSize);
    m_background->show();

    // Updates the grid.
    drawGrid();
 
    // update preview
    if (m_preview.sprite) {
        m_preview.sprite->refresh(m_renderer);
        m_preview.sprite->setPos(m_renderer->toReal(m_preview.pos));
    }
    
    // update sprites
    for (Sprites::const_iterator i = m_sprites.constBegin(); 
            i != m_sprites.constEnd();
            ++i) {
        i.value()->refresh(m_renderer);
        i.value()->setPos(m_renderer->toReal(i.key()));
    }
}

void BattleFieldView::setPreview(const QPointF& pos, Ship* ship)
{
    if (!m_preview.sprite) {
        m_preview.ship = ship;
        m_preview.sprite = m_factory.createShip(ship);
        kDebug() << "created preview: dir =" << ship->direction();
        m_preview.sprite->setOpacity(PREVIEW_OPACITY);
        scene()->addItem(m_preview.sprite);
        m_preview.sprite->stackBefore(m_screen);
    }
    
    m_preview.pos = m_renderer->toLogical(pos);
    m_preview.sprite->setPos(m_renderer->toReal(m_preview.pos));
}

void BattleFieldView::cancelPreview()
{
    delete m_preview.sprite;
    m_preview.sprite = 0;
    m_preview.ship = 0;
}

void BattleFieldView::addSprite(const Coord& c, Sprite* sprite)
{
    m_sprites.insert(c, sprite);
    
    sprite->setPos(m_renderer->toReal(c));
    scene()->addItem(sprite);
}

void BattleFieldView::add(Ship* ship)
{
    Sprite* sprite = m_factory.createShip(ship);
    addSprite(ship->position(), sprite);
    
    // fading preview in
    Animation* a = new FadeAnimation(sprite, PREVIEW_OPACITY, 1, 1000);
    Animator::instance()->add(a);

    if (ship == m_preview.ship) {
        cancelPreview();
    }
    else if (!ship->alive()) {
        Animation* a = new FadeAnimation(sprite, 0, 1, 1000);
        Animator::instance()->add(a);
    }
}

void BattleFieldView::sink(Ship* ship)
{
    m_last_hit = 0;
    Sprite* ship_sprite = 0;
    
    Coord p = ship->position();
    for (unsigned int i = 0; 
         i < ship->size(); 
         i++, p += ship->increment()) {
        foreach (Sprite* s, m_sprites.values(p)) {
            if (s->spriteKey().startsWith("ship")) {
                ship_sprite = s;
            }
            else if (s->spriteKey().startsWith("hit")) {
                s->setSpriteKey("hit-end");
                s->refresh(m_renderer);
                s->setZValue(BACKGROUND);
                s->setOpacity(0.5);
            }
        }
    }
    if (ship_sprite) {
        ship_sprite->stackBefore(m_background);
    }
}

void BattleFieldView::hit(const Coord& c)
{
    removeImpact();
    m_last_hit = m_factory.createHit();
    m_last_hit->setZValue(BACKGROUND);
    addSprite(c, m_last_hit);
}

void BattleFieldView::miss(const Coord& c)
{
    removeImpact();
    m_impact = m_factory.createImpact();
    m_impact->setZValue(BACKGROUND);
    addSprite(c, m_impact);
}

void BattleFieldView::removeImpact() {
    if (m_impact) {
        m_impact->setSpriteKey("water");
        m_impact->refresh(m_renderer);
        m_impact = 0;
    }
    if (m_last_hit) {
        m_last_hit->setSpriteKey("hit-after");
        m_last_hit->refresh(m_renderer);
        m_last_hit = 0;
    }
}

void BattleFieldView::clear()
{
    delete m_preview.sprite;
    m_preview.sprite = 0;
    m_preview.ship = 0;
    
    m_impact = 0;
    m_last_hit = 0;
    
    qDeleteAll(m_sprites);
    m_sprites.clear();
}

void BattleFieldView::mousePressEvent(QMouseEvent *ev)
{
    Button *button = dynamic_cast<Button *>(itemAt(ev->pos()));

    if (m_screen->isVisible() && button)
    {
        m_screen->onMousePress(button);
    }
    else if (ev->button() == Qt::LeftButton && m_delegate)
    {
        Coord c = m_renderer->toLogical(ev->pos());
        m_delegate->action(m_player, c);
    }
    else if (ev->button() == Qt::RightButton && m_delegate)
    {
        m_delegate->changeDirection(m_player);

        Coord c = m_renderer->toLogical(ev->pos());

        if(Ship *ship = m_delegate->canAddShip(m_player, c))
            setPreview(mapToScene(ev->pos()), ship);
    }
}

void BattleFieldView::mouseReleaseEvent(QMouseEvent *ev)
{
    Button *button = dynamic_cast<Button *>(itemAt(ev->pos()));

    if (m_screen->isVisible() && button && ev->button() == Qt::LeftButton) {
        m_screen->onMouseRelease(button);
    }
}

void BattleFieldView::mouseMoveEvent(QMouseEvent *ev)
{
    Button *button = dynamic_cast<Button *>(itemAt(ev->pos()));

    if (m_screen->isVisible() && button)
    {
        m_screen->onMouseMove(button);
    }
    else if (m_screen->isVisible() && !button)
    {
        m_screen->onMouseLeave();
    }
    else
    {
        cancelPreview();
        Coord c = m_renderer->toLogical(ev->pos());

        if(Ship *ship = m_delegate->canAddShip(m_player, c))
            setPreview(mapToScene(ev->pos()), ship);
    }
}

void BattleFieldView::leaveEvent(QEvent *)
{
    if (m_screen->isVisible()) {
        m_screen->onMouseLeave();
    } else {
        cancelPreview();
    }
}

WelcomeScreen* BattleFieldView::screen() const
{
    return m_screen;
}

void BattleFieldView::setDelegate(Delegate *c)
{
    m_delegate = c;
}

void BattleFieldView::setPlayer(Sea::Player player)
{
    m_player = player;
}

const qreal BattleFieldView::PREVIEW_OPACITY = 0.5;
