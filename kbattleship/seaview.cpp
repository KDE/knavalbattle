#include <QMouseEvent>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <kgamecanvas.h>

#include "seaview.h"
#include "battlefieldview.h"
#include "kbsrenderer.h"
#include "controller.h"

SeaView::SeaView(QWidget* parent)
: KGameCanvasWidget(parent)
{
    // create renderer
    m_renderer = new KBSRenderer(KStandardDirs::locate("appdata", "pictures/default_theme.svgz"));
    m_renderer->resize(tileSize());
    
    // create fields
    m_fields[0] = new BattleFieldView(this, m_renderer, GRID_SIZE);
    m_fields[0]->show();
    m_fields[1] = new BattleFieldView(this, m_renderer, GRID_SIZE);
    m_fields[1]->show();
    
    update();
}

void SeaView::update()
{
    int ts = tileSize();
    
    m_renderer->resize(ts);
    
    m_fields[0]->moveTo(0, 0);
    m_fields[0]->update();
    m_fields[1]->moveTo(m_fields[0]->size().width() + ts, 0);
    m_fields[1]->update();
}

void SeaView::resizeEvent(QResizeEvent*)
{
    update();
}

int SeaView::fieldAt(const QPoint& p)
{
    if (m_fields[0]->rect().contains(p)) {
        return 0;
    }
    else if (m_fields[1]->rect().contains(p)) {
        return 1;
    }
    else {
        return -1;
    }
}

void SeaView::mousePressEvent(QMouseEvent* e)
{
    if (!m_controller) {
        return;
    }
    
    int f = fieldAt(e->pos());
    if (f == -1) {
        return;
    }
    
    BattleFieldView* field = m_fields[f];
    Coord c = m_renderer->toLogical(e->pos() - field->pos());
    m_controller->action(Sea::Player(f), c);
}

void SeaView::mouseMoveEvent(QMouseEvent* e)
{
    if (!m_controller) {
        return;
    }
    
    KGameCanvasItem* item = itemAt(e->pos());
    if (static_cast<KGameCanvasItem*>(m_fields[0]) == item) {
        m_fields[1]->cancelPreview();
        setPreview(Sea::Player(0), e->pos());
    }
    else if (static_cast<KGameCanvasItem*>(m_fields[1]) == item) {
        m_fields[0]->cancelPreview();
        setPreview(Sea::Player(1), e->pos());
    }
    else {
        cancelPreview();
    }
}

void SeaView::setPreview(Sea::Player player, const QPoint& pos)
{
    QPoint p = pos - m_fields[player]->pos();
    Coord c = m_renderer->toLogical(p);
    if (Ship* ship = m_controller->canAddShip(player, c)) {
        m_fields[player]->setPreview(p, ship);
    }
    else {
        m_fields[player]->cancelPreview();
    }
}

void SeaView::cancelPreview()
{
    m_fields[0]->cancelPreview();
    m_fields[1]->cancelPreview();
}

void SeaView::add(Sea::Player p, const Coord& c, Ship* ship) 
{
    m_fields[p]->add(c, ship);
}

void SeaView::hit(Sea::Player p, const Coord& c)
{
    m_fields[p]->hit(c);
}

void SeaView::miss(Sea::Player p, const Coord& c)
{
    m_fields[p]->miss(c);
}

void SeaView::gameOver(Sea::Status status)
{
    
}

BattleFieldView* SeaView::otherField(BattleFieldView* field)
{
    return field == m_fields[0] ? m_fields[1] : m_fields[0];
}

int SeaView::tileSize() const
{
    int h = height() / GRID_SIZE;
    int w = width() / (GRID_SIZE * 2 + 1);
    return w < h ? w : h;
}

void SeaView::setController(Controller* c)
{
    m_controller = c;
    setMouseTracking(c != 0);
}

