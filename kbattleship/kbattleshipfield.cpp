#include <kdebug.h>
#include "kbattleshipfield.h"
#include "kbattlefield.h"
#include "kbsrenderer.h"

KBattleShipField::KBattleShipField(KGameCanvasAbstract* parent, KBSRenderer* renderer, int size)
        : ManagedGroup(parent)
        , m_renderer(renderer)
        , m_size(size)
        , m_grid(size, size)
{
    m_background = new KGameCanvasPixmap(m_renderer->renderBackground(), this);
    m_background->moveTo(0, 0);
    m_background->show();

    m_preview = 0;
}

int KBattleShipField::gridSize() const
{
    return m_renderer->size().width();
}

void KBattleShipField::clear()
{
}

int KBattleShipField::state(int x, int y) const
{
    return 0;
}

void KBattleShipField::setState(int x, int y, int type)
{
    createSprite(Coord(x, y), type, false, this);
}

void KBattleShipField::setPreviewState(int x, int y, int type, bool rotate)
{
    delete m_preview;
    if (valid(Coord(x, y))) {
        m_preview = createShip(x, y, type, rotate);
        m_preview->setOpacity(120);
        m_preview->show();
    } else
        m_preview = 0;
}

KGameCanvasGroup* KBattleShipField::createShip(int x, int y, int type, bool rotate)
{
    KGameCanvasGroup* ship = new ManagedGroup(this);
    Coord p(0, 0);
    ship->moveTo(x * gridSize(), y * gridSize());
    switch (type) {
    case KBattleField::SHIP1P1: {
            createSprite(p, KBattleField::SHIP1P1, rotate, ship);
            //      m_grid[p] = KFieldItem(type, sprite);
            break;
        }
    case KBattleField::SHIP2P1: {
            createSprite(p, KBattleField::SHIP2P1, rotate, ship);
            incCoord(p, rotate);
            createSprite(p, KBattleField::SHIP2P2, rotate, ship);
            //       Coord p(x, y);
            //       m_grid[p] = KFieldItem(type, sprite1);
            //       incCoord(p, rotate);
            //       m_grid[p] = KFieldItem(type, sprite2);
            break;
        }
    case KBattleField::SHIP3P1: {
            createSprite(p, KBattleField::SHIP3P1, rotate, ship);
            incCoord(p, rotate);
            createSprite(p, KBattleField::SHIP3P2, rotate, ship);
            incCoord(p, rotate);
            createSprite(p, KBattleField::SHIP3P3, rotate, ship);
            /*      Coord p(x, y);
                  m_grid[p] = KFieldItem(type, sprite1);
                  incCoord(p, rotate);
                  m_grid[p] = KFieldItem(type, sprite2);
                  incCoord(p, rotate);
                  m_grid[p] = KFieldItem(type, sprite3);*/
            break;
        }
    case KBattleField::SHIP4P1: {
            createSprite(p, KBattleField::SHIP4P1, rotate, ship);
            incCoord(p, rotate);
            createSprite(p, KBattleField::SHIP4P2, rotate, ship);
            incCoord(p, rotate);
            createSprite(p, KBattleField::SHIP4P3, rotate, ship);
            incCoord(p, rotate);
            createSprite(p, KBattleField::SHIP4P4, rotate, ship);
            /*      Coord p(x, y);
                  m_grid[p] = KFieldItem(type, sprite1);
                  incCoord(p, rotate);
                  m_grid[p] = KFieldItem(type, sprite2);
                  incCoord(p, rotate);
                  m_grid[p] = KFieldItem(type, sprite3);
                  incCoord(p, rotate);
                  m_grid[p] = KFieldItem(type, sprite4);*/
            break;
        }
    default:
        kDebug() << "error: wrong type when creating ship\n";
    }
    return ship;
}

KGameCanvasPixmap* KBattleShipField::createSprite(const Coord& p, int type, bool rotate, KGameCanvasAbstract* parent)
{
    if (valid(p)) {
        if (!parent) parent = this;
        QPixmap pix = m_renderer->render(type, rotate);
        KGameCanvasPixmap* sprite = new KGameCanvasPixmap(pix, parent);
        sprite->moveTo(p.first * gridSize(), p.second * gridSize());
        sprite->show();
        return sprite;
    } else
        return 0;
}

void KBattleShipField::incCoord(Coord& p, bool rotate) const
{
    if (rotate)
        p.second++;
    else
        p.first++;
}

bool KBattleShipField::valid(const Coord& p) const
{
    return p.first >= 0 && p.first < m_size && p.second >= 0 && p.second < m_size;
}

KBattleShipField::Coord KBattleShipField::getCoord(const QPoint& point) const
{
    QPoint rel = point - pos();
    QSize grid_size = m_renderer->size();
    int x = rel.x() / grid_size.width();
    int y = rel.y() / grid_size.height();
    return Coord(x, y);
}

