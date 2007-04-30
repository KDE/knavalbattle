/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "seaview.h"

#include <QMouseEvent>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kgamecanvas.h>

#include "battlefieldview.h"
#include "entitylabel.h"
#include "kbsrenderer.h"
#include "delegate.h"

SeaView::SeaView(QWidget* parent)
: KGameCanvasWidget(parent)
, m_delegate(0)
, m_last_f(-1)
{/*
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);*/

    // create renderer
    m_renderer = new KBSRenderer(KStandardDirs::locate("appdata", "pictures/default_theme.svgz"));
    m_renderer->resize(tileSize());
    
    // create fields
    m_fields[0] = new BattleFieldView(this, m_renderer, "background", GRID_SIZE);
    m_fields[0]->show();
    m_fields[1] = new BattleFieldView(this, m_renderer, "background2", GRID_SIZE);
    m_fields[1]->show();
    
    // create labels
    QFont labelFont = parent->font();
    labelFont.setPixelSize(LABEL_HEIGHT / 3);
    m_labels[0] = new EntityLabel(this, labelFont, "", QSize(m_fields[0]->size().width(), LABEL_HEIGHT));
    m_labels[0]->show();
    m_labels[1] = new EntityLabel(this, labelFont, "", QSize(m_fields[1]->size().width(), LABEL_HEIGHT));
    m_labels[1]->show();
    
    Animator::instance()->start();
    
    update();
    
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    
    setMouseTracking(true);
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
    
    m_labels[0]->resize(QSize(m_fields[0]->size().width(), LABEL_HEIGHT));
    m_labels[0]->moveTo(m_fields[0]->pos().x(), m_fields[0]->size().height() + 10);
    m_labels[1]->resize(QSize(m_fields[1]->size().width(), LABEL_HEIGHT));
    m_labels[1]->moveTo(m_fields[1]->pos().x(), m_fields[1]->size().height() + 10);
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
    int f = fieldAt(e->pos());
    if (f == -1) {
        return;
    }
    
    BattleFieldView* field = m_fields[f];
    QPoint point = e->pos() - field->pos();
    Coord c = m_renderer->toLogical(e->pos() - field->pos());
    
    if (e->button() == Qt::LeftButton) {
        field->onMousePress(point);
        if (m_delegate) {
            m_delegate->action(Sea::Player(f), c);
        }
    }
    else if (e->button() == Qt::RightButton) {
        if (m_delegate) {
            m_delegate->changeDirection(Sea::Player(f));
        }
    }
}

void SeaView::mouseReleaseEvent(QMouseEvent* e)
{
    int f = fieldAt(e->pos());
    if (f == -1) {
        return;
    }
    
    BattleFieldView* field = m_fields[f];
    
    if (e->button() == Qt::LeftButton) {
        field->onMouseRelease(e->pos() - field->pos());
    }
}

void SeaView::leaveEvent(QEvent*)
{
    if (m_last_f != -1) {
        BattleFieldView* field = m_fields[m_last_f];
        field->onMouseLeave();
    }
    m_last_f = -1;
}


void SeaView::mouseMoveEvent(QMouseEvent* e)
{
    // send mouse move info to the welcome screen
    int f = fieldAt(e->pos());
    if (m_last_f != -1 && m_last_f != f) {
        BattleFieldView* field = m_fields[m_last_f];
        field->onMouseLeave();
    }
    m_last_f = f;
    if (f != -1) {
        BattleFieldView* field = m_fields[f];
        field->onMouseMove(e->pos() - field->pos());

    }
    
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

void SeaView::sink(Sea::Player p, const Coord& c, Ship* ship)
{
    m_fields[p]->sink(c, ship);
}

void SeaView::clear()
{
    m_fields[0]->clear();
    m_fields[1]->clear();
    Animator::instance()->restart();
}

BattleFieldView* SeaView::otherField(BattleFieldView* field)
{
    return field == m_fields[0] ? m_fields[1] : m_fields[0];
}

int SeaView::tileSize() const
{
    int h = (height() - LABEL_HEIGHT) / GRID_SIZE;
    int w = width() / (GRID_SIZE * 2 + 1);
    return w < h ? w : h;
}

void SeaView::setDelegate(Delegate* c)
{
    kDebug() << "setting delegate to " << c << endl;
    m_delegate = c;
}

QSize SeaView::sizeHint() const
{
    return QSize(100, 400);
}

WelcomeScreen* SeaView::screen(Sea::Player p) const
{
    return m_fields[p]->screen();
}

