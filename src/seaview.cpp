/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "seaview.h"

#include <QMouseEvent>
#include <QObject>

#include <QIcon>

#include "battlefieldview.h"
#include "button.h"
#include "delegate.h"
#include "kbsrenderer.h"
#include "playerlabel.h"
#include "statswidget.h"
#include "welcomescreen.h"


SeaView::SeaView(QWidget* parent)
: QWidget(parent)
, m_delegate(0)
, m_last_f(-1)
{/*
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);*/

    // create renderer
    m_renderer = new KBSRenderer();
   
    // create labels
    m_labels[0] = new PlayerLabel(
        m_renderer->spritePixmap(QStringLiteral("score_mouse"), QSize(32, 32)),
        QStringLiteral("Player"),
        this);
    m_labels[0]->show();
    m_labels[1] = new PlayerLabel(
        m_renderer->spritePixmap(QStringLiteral("score_ai"), QSize(32, 32)),
        QStringLiteral("Computer"), 
        this);
    m_labels[1]->show();
 
    // create fields
    m_fields[0] = new BattleFieldView(this, m_renderer, QStringLiteral("background"), GRID_SIZE);
    m_fields[0]->setPlayer(Sea::Player(0));
    m_fields[0]->show();
    m_fields[1] = new BattleFieldView(this, m_renderer, QStringLiteral("background2"), GRID_SIZE);
    m_fields[1]->setPlayer(Sea::Player(1));
    m_fields[1]->show();
    
    // create stats widgets
    m_stats[0] = new StatsWidget(m_renderer, this);
    m_stats[0]->show();
    
    m_stats[1] = new StatsWidget(m_renderer, this);
    m_stats[1]->show();
    
    Animator::instance()->start();
    refresh();
    
    connect(screen(Sea::Player(0)), &WelcomeScreen::hidden, this, &SeaView::refresh);
    connect(screen(Sea::Player(0)), &WelcomeScreen::shown, this, &SeaView::refresh);
}

SeaView::~SeaView() {
	delete m_renderer;
}

void SeaView::refresh()
{
    QSize ts = tileSize();
    m_renderer->resize(ts);

    m_fields[0]->refresh();
    m_fields[1]->refresh();

    int actual_width = 
        m_fields[0]->size().width() +
        m_fields[1]->size().width() +
        GAP;
 
    m_fields[0]->move((width() - actual_width) / 2, 
                      m_labels[0]->height() + LABEL_SPACING * 2);

    if (!screen(Sea::Player(0))->isVisible()) {
        m_labels[0]->show();
        m_labels[0]->move(m_fields[0]->pos().x(), LABEL_SPACING);
        m_labels[0]->update();
    }
    else {
        m_labels[0]->hide();
    }
    m_stats[0]->move(m_fields[0]->pos().x(), 
                     m_fields[0]->pos().y() + m_fields[0]->height() + 2);
    m_stats[0]->setWidth(m_fields[0]->size().width());
    m_stats[0]->refresh();
 
    m_fields[1]->move(
        m_fields[0]->pos().x() + m_fields[0]->size().width() + GAP, 
        m_fields[0]->pos().y());
    if (!screen(Sea::Player(0))->isVisible()) {
        m_labels[1]->show();
        m_labels[1]->move(m_fields[1]->pos().x(), LABEL_SPACING);
        m_labels[1]->update();
    }
    else {
        m_labels[1]->hide();
    }
    m_stats[1]->move(m_fields[1]->pos().x(), 
                     m_fields[1]->pos().y() + m_fields[1]->height() + 2);
    m_stats[1]->setWidth(m_fields[0]->size().width());
    m_stats[1]->refresh();
}

void SeaView::resizeEvent(QResizeEvent*)
{
    refresh();
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

bool SeaView::updatePreview(const QPoint& pos)
{
    QWidget* item = childAt(pos);
    if (static_cast<QGraphicsView*>(m_fields[0]) == item) {
        m_fields[1]->cancelPreview();
        if (setPreview(Sea::Player(0), pos)) {
            m_last_preview = pos;
        }
        return true;
    }
    else if (static_cast<QGraphicsView*>(m_fields[1]) == item) {
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

    m_fields[player]->setPreview(p);
    return m_delegate->canAddShip(player, c);
}

void SeaView::cancelPreview()
{
    m_fields[0]->cancelPreview();
    m_fields[1]->cancelPreview();
    updatePreview(m_last_preview);
}

void SeaView::add(Sea::Player p, Ship* ship)
{
    m_fields[p]->add(ship);
}

void SeaView::hit(Sea::Player p, const Coord& c)
{
    m_fields[p]->hit(c);
}

void SeaView::miss(Sea::Player p, const Coord& c)
{
    m_fields[p]->miss(c);
}

void SeaView::sink(Sea::Player p, Ship* ship)
{
    m_fields[p]->sink(ship);
}

void SeaView::setStatus(Sea::Status status)
{
    m_fields[0]->setStatus(status);
    m_fields[1]->setStatus(status);
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

QSize SeaView::tileSize() const
{
    int h = (height() -
        m_labels[0]->height() -
        LABEL_SPACING * 2 -
        m_stats[0]->height() -
        MARGIN * 2) / GRID_SIZE;
    int w = (width() - GAP - MARGIN * 2) / (GRID_SIZE * 2);
    return w < h ? QSize(w, w) : QSize(h, h);
}

void SeaView::setDelegate(Delegate* c)
{
    m_fields[0]->setDelegate(c);
    m_fields[1]->setDelegate(c);
}

WelcomeScreen* SeaView::globalScreen() const
{
    return m_screen;
}

WelcomeScreen* SeaView::screen(Sea::Player player) const
{
    return m_fields[player]->screen();
}

void SeaView::toggleLeftGrid(bool show)
{
    m_fields[0]->toggleGrid(show);
}

void SeaView::toggleRightGrid(bool show)
{
    m_fields[1]->toggleGrid(show);
}

void SeaView::setStats(Sea::Player p, const QString& icon, 
                       const QString& text, Stats* stats)
{
    m_labels[p]->setData(m_renderer->spritePixmap(icon, QSize(32, 32)), text);
    m_stats[p]->setData(stats);
    
}

void SeaView::setNick(Sea::Player p, const QString& nick)
{
    m_labels[p]->setText(nick);
}



