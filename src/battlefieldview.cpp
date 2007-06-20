/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "battlefieldview.h"

#include <kdebug.h>
#include <kicon.h>
#include <QList>

#include "kbsrenderer.h"
#include "sprite.h"
#include "animator.h"
#include "animation.h"
#include "welcomescreen.h"

BattleFieldView::BattleFieldView(KGameCanvasWidget* parent, KBSRenderer* renderer, const QString& bgID, int gridSize)
: KGameCanvasGroup(parent)
, m_renderer(renderer)
, m_factory(this, renderer)
, m_bgID(bgID)
, m_gridSize(gridSize)
, m_impact(0)
, m_last_hit(0)
{
    m_background_lower = new KGameCanvasPixmap(
        m_renderer->render(bgID + "-layer1", false, m_gridSize, m_gridSize), this);
    m_background_lower->moveTo(0, 0);
    m_background_lower->setOpacity(250);
    m_background_lower->show();
    
    m_background = new KGameCanvasPixmap(
        m_renderer->render(bgID + "-layer2", false, m_gridSize, m_gridSize), this);
    m_background->moveTo(0, 0);
    m_background->setOpacity(250);
    m_background->stackOver(m_background_lower);
    m_background->show();
    
    m_screen = new WelcomeScreen(this, parent->font());
    m_screen->stackOver(m_background);
//     m_screen->show();
    
    for (Sprites::iterator i = m_sprites.begin();
            i != m_sprites.end();
            ++i) {
        i.value() = 0;
    }
}

QSize BattleFieldView::size() const
{
    return m_renderer->size() * m_gridSize;
}

void BattleFieldView::update()
{
    // update welcome screen
    m_screen->moveTo(0, 0);
    m_screen->resize(size());

    // update background
    m_background_lower->setPixmap(
        m_renderer->render(m_bgID + "-layer1", false, m_gridSize, m_gridSize));
    m_background_lower->moveTo(0, 0);
    m_background->setPixmap(
        m_renderer->render(m_bgID + "-layer2", false, m_gridSize, m_gridSize));
    m_background->moveTo(0, 0);
    
    // update preview
    if (m_preview.sprite) {
        m_preview.sprite->update(m_renderer);
        m_preview.sprite->moveTo(m_renderer->toReal(m_preview.pos));
    }
    
    // update sprites
    for (Sprites::const_iterator i = m_sprites.constBegin(); 
            i != m_sprites.constEnd();
            ++i) {
        i.value()->update(m_renderer);
        i.value()->moveTo(m_renderer->toReal(i.key()));
    }
}

void BattleFieldView::setPreview(const QPoint& pos, Ship* ship)
{
    if (!m_preview.sprite) {
        m_preview.ship = ship;
        m_preview.sprite = m_factory.createShip(ship);
        kDebug() << "created preview: dir = " << ship->direction() << endl;
        m_preview.sprite->setOpacity(PREVIEW_OPACITY);
        m_preview.sprite->show();
    }
    
    m_preview.pos = m_renderer->toLogical(pos);
    m_preview.sprite->moveTo(m_renderer->toReal(m_preview.pos));
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
    
    sprite->moveTo(m_renderer->toReal(c));
    sprite->show();
}

void BattleFieldView::add(const Coord& c, Ship* ship)
{
    Sprite* sprite = m_factory.createShip(ship);
    addSprite(c, sprite);
    
    sprite->stackOver(m_background);
    
    // fading preview in
    if (ship == m_preview.ship) {
        Animation* a = new FadeAnimation(sprite, PREVIEW_OPACITY, 255, 1000);
        Animator::instance()->add(a);
        cancelPreview();
    }
    else if (!ship->alive()) {
        Animation* a = new FadeAnimation(sprite, 0, 200, 1000);
        Animator::instance()->add(a);
    }
}

void BattleFieldView::sink(const Coord& c, Ship* ship)
{
    m_last_hit = 0;
    Sprite* ship_sprite = 0;
    
    Coord p = c;
    for (unsigned int i = 0; 
         i < ship->size(); 
         i++, p += ship->increment()) {
        foreach (Sprite* s, m_sprites.values(p)) {
            if (s->name().startsWith("ship")) {
                ship_sprite = s;
            }
            else if (s->name().startsWith("hit")) {
                s->setName("hit-end");
                s->update(m_renderer);
                s->stackOver(m_background_lower);
            }
        }
    }
    if (ship_sprite) {
        ship_sprite->stackOver(m_background_lower);
    }
}

void BattleFieldView::hit(const Coord& c)
{
    removeImpact();
    m_last_hit = m_factory.createHit();
    addSprite(c, m_last_hit);
}

void BattleFieldView::miss(const Coord& c)
{
    removeImpact();
    m_impact = m_factory.createImpact();
    addSprite(c, m_impact);
}

void BattleFieldView::removeImpact() {
    if (m_impact) {
        m_impact->setName("water");
        m_impact->update(m_renderer);
        m_impact = 0;
    }
    if (m_last_hit) {
        m_last_hit->setName("hit-after");
        m_last_hit->update(m_renderer);
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
    
    foreach (Sprite* s, m_sprites) {
        delete s;
    }
    m_sprites.clear();
}

void BattleFieldView::onMousePress(const QPoint& p)
{
    if (m_screen->active()) {
        m_screen->onMousePress(p);
    }
}

void BattleFieldView::onMouseRelease(const QPoint& p)
{
    if (m_screen->active()) {
        m_screen->onMouseRelease(p);
    }
}

void BattleFieldView::onMouseMove(const QPoint& p)
{
    if (m_screen->active()) {
        m_screen->onMouseMove(p);
    }
}

void BattleFieldView::onMouseLeave()
{
    if (m_screen->active()) {
        m_screen->onMouseLeave();
    }
}

WelcomeScreen* BattleFieldView::screen() const
{
    return m_screen;
}
