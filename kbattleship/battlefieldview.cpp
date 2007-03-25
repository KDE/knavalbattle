#include <kdebug.h>
#include <QList>

#include "battlefieldview.h"
#include "kbsrenderer.h"
#include "sprite.h"
#include "animator.h"
#include "animation.h"

BattleFieldView::BattleFieldView(KGameCanvasAbstract* parent, KBSRenderer* renderer, 
                                Animator* animator, int gridSize)
: KGameCanvasGroup(parent)
, m_renderer(renderer)
, m_animator(animator)
, m_factory(this, renderer)
, m_gridSize(gridSize)
{
    m_background = new KGameCanvasPixmap(m_renderer->render("background", false, m_gridSize, m_gridSize), this);
    m_background->moveTo(0, 0);
    m_background->setOpacity(250);
    m_background->show();
    
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
    // update background
    m_background->setPixmap(m_renderer->render("background", false, m_gridSize, m_gridSize));
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
    kDebug() << "adding sprite " << sprite << endl;
    kDebug() << "background = " << m_background << endl;
    addSprite(c, sprite);
    
    
    foreach (KGameCanvasItem* item, *items()) {
        kDebug() << "item = " << item << endl;
    }
    
    sprite->stackOver(m_background);
    
    foreach (KGameCanvasItem* item, *items()) {
        kDebug() << "item = " << item << endl;
    }
    
    // fading preview in
    if (ship == m_preview.ship) {
        Animation* a = new FadeAnimation(sprite, PREVIEW_OPACITY, 255, 1000);
        m_animator->add(a);
    }
    else if (!ship->alive()) {
        Animation* a = new FadeAnimation(sprite, 0, 200, 1000);
        m_animator->add(a);
    }
}

void BattleFieldView::hit(const Coord& c)
{
    addSprite(c, m_factory.createHit());
}

void BattleFieldView::miss(const Coord& c)
{
    addSprite(c, m_factory.createMiss());
}

