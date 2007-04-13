/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include <QMouseEvent>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kgamecanvas.h>

#include "seaview.h"
#include "battlefieldview.h"
#include "kbsrenderer.h"
#include "delegate.h"

SeaView::SeaView(QWidget* parent)
: KGameCanvasWidget(parent)
, m_delegate(0)
{
    // create renderer
    m_renderer = new KBSRenderer(KStandardDirs::locate("appdata", "pictures/default_theme.svgz"));
    m_renderer->resize(tileSize());
    
    // create fields
    m_fields[0] = new BattleFieldView(this, m_renderer, &m_animator, "background", GRID_SIZE);
    m_fields[0]->show();
    m_fields[1] = new BattleFieldView(this, m_renderer, &m_animator, "background2", GRID_SIZE);
    m_fields[1]->show();
    
    m_animator.start();
    
    update();
    
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
}

SeaView::~SeaView() {
	delete m_renderer;
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
    kDebug() << "x = " << rect().x() << endl;
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
    if (!m_delegate) {
        return;
    }
    
    int f = fieldAt(e->pos());
    if (f == -1) {
        return;
    }
    
    BattleFieldView* field = m_fields[f];
    Coord c = m_renderer->toLogical(e->pos() - field->pos());
    
    if (e->button() == Qt::LeftButton) {
        m_delegate->action(Sea::Player(f), c);
    }
    else if (e->button() == Qt::RightButton) {
        m_delegate->changeDirection(Sea::Player(f));
    }
}

void SeaView::mouseMoveEvent(QMouseEvent* e)
{
    if (!m_delegate) {
        return;
    }
    
    if (!updatePreview(e->pos())) {
        m_fields[0]->cancelPreview();
        m_fields[1]->cancelPreview();
    }
}

bool SeaView::updatePreview(const QPoint& pos)
{
    KGameCanvasItem* item = itemAt(pos);
    if (static_cast<KGameCanvasItem*>(m_fields[0]) == item) {
        m_fields[1]->cancelPreview();
        if (setPreview(Sea::Player(0), pos)) {
            m_last_preview = pos;
        }
        return true;
    }
    else if (static_cast<KGameCanvasItem*>(m_fields[1]) == item) {
        m_fields[0]->cancelPreview();
        if (setPreview(Sea::Player(1), pos)) {
            m_last_preview = pos;
        }
        return true;
    }
    else {
        return false;
    }
}

bool SeaView::setPreview(Sea::Player player, const QPoint& pos)
{
    QPoint p = pos - m_fields[player]->pos();
    Coord c = m_renderer->toLogical(p);
    if (Ship* ship = m_delegate->canAddShip(player, c)) {
        m_fields[player]->setPreview(p, ship);
        return true;
    }
    else {
        m_fields[player]->cancelPreview();
        return false;
    }
}

void SeaView::cancelPreview()
{
    m_fields[0]->cancelPreview();
    m_fields[1]->cancelPreview();
    updatePreview(m_last_preview);
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

void SeaView::clear()
{
    m_fields[0]->clear();
    m_fields[1]->clear();
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

void SeaView::setDelegate(Delegate* c)
{
    kDebug() << "setting delegate to " << c << endl;
    m_delegate = c;
    setMouseTracking(c != 0);
}

QSize SeaView::sizeHint() const
{
    return QSize(100, 400);
}

