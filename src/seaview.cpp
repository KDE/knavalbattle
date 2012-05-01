/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "seaview.h"

#include <QMouseEvent>

#include <KIcon>
#include <KStandardDirs>

#include "battlefieldview.h"
#include "button.h"
#include "delegate.h"
#include "kbsrenderer.h"
#include "playerlabel.h"
#include "statswidget.h"
#include "welcomescreen.h"


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
    
    // create screen
    m_screen = new WelcomeScreen(this, font());
    m_screen->hide();
    
    // create labels
    m_labels[0] = new PlayerLabel(
        m_renderer->render("score_mouse", QSize(32, 32)),
        "Player",
        this);
    m_labels[0]->stackUnder(m_screen);
    m_labels[0]->show();
    m_labels[1] = new PlayerLabel(
        m_renderer->render("score_ai", QSize(32, 32)),
        "Computer", 
        this);
    m_labels[1]->stackUnder(m_screen);
    m_labels[1]->show();
    
    // create fields
    m_fields[0] = new BattleFieldView(this, m_renderer, "background", GRID_SIZE);
    m_fields[0]->stackUnder(m_screen);
    m_fields[0]->show();
    connect(m_fields[0]->screen(), SIGNAL(clicked(Button*)), this, SLOT(buttonClicked(Button*)));
    m_fields[1] = new BattleFieldView(this, m_renderer, "background2", GRID_SIZE);
    m_fields[1]->stackUnder(m_screen);
    m_fields[1]->show();
    connect(m_fields[1]->screen(), SIGNAL(clicked(Button*)), this, SLOT(buttonClicked(Button*)));
    
    // create stats widgets
    m_stats[0] = new StatsWidget(m_fields[0]->size().width(), m_renderer, this);
    m_stats[0]->stackUnder(m_screen);
    m_stats[0]->show();
    
    m_stats[1] = new StatsWidget(m_fields[1]->size().width(), m_renderer, this);
    m_stats[1]->stackUnder(m_screen);
    m_stats[1]->show();
    
    
    Animator::instance()->start();
    update();
    
    connect(screen(Sea::Player(0)), SIGNAL(hidden()), this, SLOT(update()));
    connect(screen(Sea::Player(0)), SIGNAL(shown()), this, SLOT(update()));
    
    setMouseTracking(true);
}

SeaView::~SeaView() {
	delete m_renderer;
}

void SeaView::update()
{
    int ts = tileSize();
    
    m_renderer->resize(ts);

    m_fields[0]->update();
    m_fields[1]->update();
    int actual_width = 
        m_fields[0]->size().width() +
        m_fields[1]->size().width() +
        GAP;
    
    m_fields[0]->moveTo(
        (width() - actual_width) / 2, 
        PlayerLabel::HEIGHT + LABEL_SPACING * 2);
    if (!screen(Sea::Player(0))->active()) {
        m_labels[0]->show();
        m_labels[0]->moveTo(m_fields[0]->pos().x(), LABEL_SPACING);
        m_labels[0]->update();
    }
    else {
        m_labels[0]->hide();
    }
    m_stats[0]->moveTo(m_fields[0]->pos().x(), 
        height() - StatsWidget::HEIGHT - MARGIN);
    m_stats[0]->setWidth(m_fields[0]->size().width());
    m_stats[0]->update();
    
    m_fields[1]->moveTo(
        m_fields[0]->pos().x() + m_fields[0]->size().width() + GAP, 
        m_fields[0]->pos().y());
    if (!screen(Sea::Player(0))->active()) {
        m_labels[1]->show();
        m_labels[1]->moveTo(m_fields[1]->pos().x(), LABEL_SPACING);
        m_labels[1]->update();
    }
    else {
        m_labels[1]->hide();
    }
    m_stats[1]->moveTo(m_fields[1]->pos().x(), 
        height() - StatsWidget::HEIGHT - MARGIN);
    m_stats[1]->setWidth(m_fields[0]->size().width());
    m_stats[1]->update();
    
    m_screen->moveTo(0, 0);
    m_screen->resize(QSize(m_fields[1]->pos().x() + m_fields[1]->size().width(),
                           m_fields[0]->size().height()));
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
    if (m_screen->active()) {
        m_screen->onMousePress(e->pos());
    }

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
    if (m_screen->active()) {
        m_screen->onMouseRelease(e->pos());
    }

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
    if (m_screen->active()) {
        m_screen->onMouseLeave();
    }
    
    if (m_last_f != -1) {
        BattleFieldView* field = m_fields[m_last_f];
        field->onMouseLeave();
    }
    m_last_f = -1;
}


void SeaView::mouseMoveEvent(QMouseEvent* e)
{
    if (m_screen->active()) {
        m_screen->onMouseMove(e->pos());
    }
    
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
}

BattleFieldView* SeaView::otherField(BattleFieldView* field)
{
    return field == m_fields[0] ? m_fields[1] : m_fields[0];
}

int SeaView::tileSize() const
{
    int h = (height() -
        PlayerLabel::HEIGHT -
        LABEL_SPACING * 2 -
        StatsWidget::HEIGHT -
        MARGIN * 2) / GRID_SIZE;
    int w = (width() - GAP) / (GRID_SIZE * 2);
    return w < h ? w : h;
}

void SeaView::setDelegate(Delegate* c)
{
    m_delegate = c;
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
    m_fields[0]->drawGrid(show);
}

void SeaView::toggleRightGrid(bool show)
{
    m_fields[1]->drawGrid(show);
}


void SeaView::buttonClicked(Button* button)
{
    Q_UNUSED( button );
}

void SeaView::setStats(Sea::Player p, const QString& icon, 
                       const QString& text, Stats* stats)
{
    m_labels[p]->setData(m_renderer->render(icon, QSize(32, 32)), text);
    m_stats[p]->setData(stats);
    
}

void SeaView::setNick(Sea::Player p, const QString& nick)
{
    m_labels[p]->setText(nick);
}

#include "seaview.moc"

