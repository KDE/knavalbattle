#include <kdebug.h>

#include "battlefieldview.h"
#include "kbsrenderer.h"
#include "sprite.h"

#define FOREACH_SQUARE(p, grid) \
    for (Coord p(0, 0); p.x < grid.width(); p.x++) \
    for (p.y = 0; p.y < grid.height(); p.y++)

BattleFieldView::BattleFieldView(KGameCanvasAbstract* parent, KBSRenderer* renderer, int gridSize)
: KGameCanvasGroup(parent)
, m_renderer(renderer)
, m_factory(this, renderer)
, m_gridSize(gridSize)
, m_sprites(Coord(gridSize, gridSize))
{
    m_background = new KGameCanvasPixmap(m_renderer->render("background", m_gridSize, m_gridSize), this);
    m_background->moveTo(0, 0);
    m_background->show();
    
    FOREACH_SQUARE(p, m_sprites) {
        kDebug() << "setting up " << p << endl;
        m_sprites[p] = 0;
    }
}

QSize BattleFieldView::size() const
{
    return m_renderer->size() * m_gridSize;
}

void BattleFieldView::update()
{
    // update background
    m_background->setPixmap(m_renderer->render("background", m_gridSize, m_gridSize));
    m_background->moveTo(0, 0);
    
    // update preview
    if (m_preview.sprite) {
        m_preview.sprite->update(m_renderer);
        m_preview.sprite->moveTo(m_renderer->toReal(m_preview.pos));
    }
    
    // update sprites
    FOREACH_SQUARE(p, m_sprites) {
        if (m_sprites[p]) {
            m_sprites[p]->update(m_renderer);
            m_sprites[p]->moveTo(m_renderer->toReal(p));
        }
    }
}

void BattleFieldView::setPreview(const QPoint& pos, Ship* ship)
{
    if (!m_preview.sprite) {
        m_preview.sprite = m_factory.createShip(ship);
        kDebug() << "created preview: size = " << ship->size() << endl;
        m_preview.sprite->setOpacity(150);
        m_preview.sprite->show();
    }
    
    m_preview.pos = m_renderer->toLogical(pos);
    m_preview.sprite->moveTo(m_renderer->toReal(m_preview.pos));
}

void BattleFieldView::cancelPreview()
{
    delete m_preview.sprite;
    m_preview.sprite = 0;
}

void BattleFieldView::addSprite(const Coord& c, Sprite* sprite)
{
    delete m_sprites[c];
    m_sprites[c] = sprite;
    
    sprite->moveTo(m_renderer->toReal(c));
    sprite->show();
}

void BattleFieldView::add(const Coord& c, Ship* ship)
{
    addSprite(c, m_factory.createShip(ship));
}

void BattleFieldView::hit(const Coord& c)
{
    addSprite(c, m_factory.createHit());
}

void BattleFieldView::miss(const Coord& c)
{
    addSprite(c, m_factory.createMiss());
}

