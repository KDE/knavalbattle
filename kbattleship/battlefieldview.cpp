#include <kdebug.h>

#include "battlefieldview.h"
#include "kbsrenderer.h"
#include "shippixmap.h"

BattleFieldView::BattleFieldView(KGameCanvasAbstract* parent, KBSRenderer* renderer, int gridSize)
: KGameCanvasGroup(parent)
, m_renderer(renderer)
, m_gridSize(gridSize)
, m_preview(0)
{
    m_background = new KGameCanvasPixmap(m_renderer->render("background", m_gridSize, m_gridSize), this);
    m_background->moveTo(0, 0);
    m_background->show();
}

QSize BattleFieldView::size() const
{
    return m_renderer->size() * m_gridSize;
}

void BattleFieldView::update()
{
    if (m_preview) {
        m_preview->update(m_renderer);
    }
    m_background->setPixmap(m_renderer->render("background", m_gridSize, m_gridSize));
}

void BattleFieldView::setPreview(const QPoint& pos, Ship* ship)
{
    if (!m_preview) {
        m_preview = new ShipPixmap(this, ship, m_renderer);
        kDebug() << "created preview: size = " << ship->size() << endl;
        m_preview->setOpacity(150);
        m_preview->show();
    }
    QSize sz = m_renderer->size();
    
    QPoint p(
        pos.x() - pos.x() % sz.width(),
        pos.y() - pos.y() % sz.height());
    m_preview->moveTo(p);
}

void BattleFieldView::cancelPreview()
{
    delete m_preview;
    m_preview = 0;
}

void BattleFieldView::add(const Coord& c, Ship* ship)
{
    QPoint p = m_renderer->toReal(c);
    ShipPixmap* pixmap = new ShipPixmap(this, ship, m_renderer);
    pixmap->moveTo(p);
    pixmap->show();
}

void BattleFieldView::addSprite(const Coord& c, const QString& name)
{
    KGameCanvasPixmap* sprite = new KGameCanvasPixmap(this);
    sprite->setPixmap(m_renderer->render(name));
    sprite->moveTo(m_renderer->toReal(c));
    sprite->show();
}

void BattleFieldView::hit(const Coord& c)
{
    addSprite(c, "hit");
}

void BattleFieldView::miss(const Coord& c)
{
    addSprite(c, "water");
}

