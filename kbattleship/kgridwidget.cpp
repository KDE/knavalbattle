/***************************************************************************
                              kgridwidget.cpp
                             -------------------
    Developers: (c) 2000-2001 Nikolas Zimmermann <wildfox@kde.org>
                (c) 2000-2001 Daniel Molkentin <molkentin@kde.org>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qpainter.h>
#include <qimage.h>

#include <kstddirs.h>
#include <kimageeffect.h>
#include <kdebug.h>

#include "kbattlefield.h"
#include "kgridwidget.moc"

KGridWidget::KGridWidget(QWidget *parent, const char *name, bool draw) : QObject(parent, name), m_drawGrid(draw)
{
    m_doubleBuffer = new QPixmap(parent->width(), parent->height());

    cleanBuffer();
    cacheImages();
}

KGridWidget::~KGridWidget()
{
}

void KGridWidget::cacheImages()
{
    seaPng = QPixmap(findIcon("sea.png"));
    waterPng = QPixmap(findIcon("water.png"));
    hitPng = QPixmap(findIcon("hit.png"));
    deathPng = QPixmap(findIcon("death.png"));
    ship1p1Png = QPixmap(findIcon("ship1-1.png"));
    ship1p1rPng = QPixmap(findIcon("ship1-1-r.png"));
    ship2p1Png = QPixmap(findIcon("ship2-1.png"));
    ship2p1rPng = QPixmap(findIcon("ship2-1-r.png"));
    ship2p2Png = QPixmap(findIcon("ship2-2.png"));
    ship2p2rPng = QPixmap(findIcon("ship2-2-r.png"));
    ship3p1Png = QPixmap(findIcon("ship3-1.png"));
    ship3p1rPng = QPixmap(findIcon("ship3-1-r.png"));
    ship3p2Png = QPixmap(findIcon("ship3-2.png"));
    ship3p2rPng = QPixmap(findIcon("ship3-2-r.png"));
    ship3p3Png = QPixmap(findIcon("ship3-3.png"));
    ship3p3rPng = QPixmap(findIcon("ship3-3-r.png"));
    ship4p1Png = QPixmap(findIcon("ship4-1.png"));
    ship4p1rPng = QPixmap(findIcon("ship4-1-r.png"));
    ship4p2Png = QPixmap(findIcon("ship4-2.png"));
    ship4p2rPng = QPixmap(findIcon("ship4-2-r.png"));
    ship4p3Png = QPixmap(findIcon("ship4-3.png"));
    ship4p3rPng = QPixmap(findIcon("ship4-3-r.png"));
    ship4p4Png = QPixmap(findIcon("ship4-4.png"));
    ship4p4rPng = QPixmap(findIcon("ship4-4-r.png"));
}

void KGridWidget::setValues(int x, int y, int size)
{
    m_x = x;
    m_y = y;
    m_size = size;
}

void KGridWidget::drawSquare()
{
    drawIcon(seaPng);
}

void KGridWidget::drawWaterIcon()
{
    drawIcon(waterPng);
}

void KGridWidget::drawHitIcon()
{
    drawIcon(hitPng);
}

void KGridWidget::drawDeathIcon()
{
    drawIcon(deathPng);
}

void KGridWidget::drawShipIcon(int type, bool rotate, bool hit, bool water)
{
    int ship = 0;
    int part = 0;

    switch(type)
    {
	case KBattleField::SHIP1P1:
	    ship = 1;
	    part = 1;
	    break;
	
	case KBattleField::SHIP2P1:
	    ship = 2;
	    if(!rotate)
		part = 1;
	    else
		part = 2;
	    break;

	case KBattleField::SHIP2P2:
	    ship = 2;
	    if(!rotate)
		part = 2;
	    else
		part = 1;
	    break;
	    
	case KBattleField::SHIP3P1:
	    ship = 3;
	    if(!rotate)
		part = 1;
	    else
		part = 3;
	    break;

	case KBattleField::SHIP3P2:
	    ship = 3;
	    if(!rotate)
		part = 2;
	    else
		part = 2;
	    break;
	    
	case KBattleField::SHIP3P3:
	    ship = 3;
	    if(!rotate)
		part = 3;
	    else
		part = 1;
	    break;
	    
	case KBattleField::SHIP4P1:
	    ship = 4;
	    if(!rotate)
		part = 1;
	    else
		part = 4;
	    break;
	    
	case KBattleField::SHIP4P2:
	    ship = 4;
	    if(!rotate)
		part = 2;
	    else
		part = 3;
	    break;

	case KBattleField::SHIP4P3:
	    ship = 4;
	    if(!rotate)
		part = 3;
	    else
		part = 2;
	    break;

	case KBattleField::SHIP4P4:
	    ship = 4;
	    if(!rotate)
		part = 4;
	    else
		part = 1;
	    break;
    }

    switch(ship)
    {
	case 1:
	    if(!rotate)
		drawIcon(ship1p1Png, hit, water);
	    else
		drawIcon(ship1p1rPng, hit, water, rotate);
	    break;

	case 2:
	    switch(part)
	    {
		case 1:
		    if(!rotate)
			drawIcon(ship2p1Png, hit, water);
		    else
			drawIcon(ship2p1rPng, hit, water, rotate);
		    break;

		case 2:
		    if(!rotate)
			drawIcon(ship2p2Png, hit, water);
		    else
			drawIcon(ship2p2rPng, hit, water, rotate);
		    break;
	    }
	    break;

	case 3:
	    switch(part)
	    {
		case 1:
		    if(!rotate)
			drawIcon(ship3p1Png, hit, water);
		    else
			drawIcon(ship3p1rPng, hit, water, rotate);
		    break;

		case 2:
		    if(!rotate)
			drawIcon(ship3p2Png, hit, water);
		    else
			drawIcon(ship3p2rPng, hit, water, rotate);
		    break;

		case 3:
		    if(!rotate)
			drawIcon(ship3p3Png, hit, water);
		    else
			drawIcon(ship3p3rPng, hit, water, rotate);
		    break;
	    }
	    break;

	case 4:
	    switch(part)
	    {
		case 1:
		    if(!rotate)
			drawIcon(ship4p1Png, hit, water);
		    else
			drawIcon(ship4p1rPng, hit, water, rotate);
		    break;

		case 2:
		    if(!rotate)
			drawIcon(ship4p2Png, hit, water);
		    else
			drawIcon(ship4p2rPng, hit, water, rotate);
		    break;

		case 3:
		    if(!rotate)
			drawIcon(ship4p3Png, hit, water);
		    else
			drawIcon(ship4p3rPng, hit, water, rotate);
		    break;

		case 4:
		    if(!rotate)
			drawIcon(ship4p4Png, hit, water);
		    else
			drawIcon(ship4p4rPng, hit, water, rotate);
		    break;
	    }
	    break;
    }
}

void KGridWidget::drawShipIcon(int ship, int part, bool rotate, bool hit)
{
    switch(ship)
    {
	case 1:
	    if(!rotate)
		drawIcon(ship1p1Png, hit);
	    else
		drawIcon(ship1p1rPng, hit, false, rotate);
	    break;

	case 2:
	    switch(part)
	    {
		case 1:
		    if(!rotate)
			drawIcon(ship2p1Png, hit);
		    else
			drawIcon(ship2p1rPng, hit, false, rotate);
		    break;

		case 2:
		    if(!rotate)
			drawIcon(ship2p2Png, hit);
		    else
			drawIcon(ship2p2rPng, hit, false, rotate);
		    break;
	    }
	    break;

	case 3:
	    switch(part)
	    {
		case 1:
		    if(!rotate)
			drawIcon(ship3p1Png, hit);
		    else
			drawIcon(ship3p1rPng, hit, false, rotate);
		    break;

		case 2:
		    if(!rotate)
			drawIcon(ship3p2Png, hit);
		    else
			drawIcon(ship3p2rPng, hit, false, rotate);
		    break;

		case 3:
		    if(!rotate)
			drawIcon(ship3p3Png, hit);
		    else
			drawIcon(ship3p3rPng, hit, false, rotate);
		    break;
	    }
	    break;

	case 4:
	    switch(part)
	    {
		case 1:
		    if(!rotate)
			drawIcon(ship4p1Png, hit);
		    else
			drawIcon(ship4p1rPng, hit, false, rotate);
		    break;

		case 2:
		    if(!rotate)
			drawIcon(ship4p2Png, hit);
		    else
			drawIcon(ship4p2rPng, hit, false, rotate);
		    break;

		case 3:
		    if(!rotate)
			drawIcon(ship4p3Png, hit);
		    else
			drawIcon(ship4p3rPng, hit, false, rotate);
		    break;

		case 4:
		    if(!rotate)
			drawIcon(ship4p4Png, hit);
		    else
			drawIcon(ship4p4rPng, hit, false, rotate);
		    break;
	    }
	    break;
    }
}

void KGridWidget::drawIcon(QPixmap icon, bool hitBlend, bool waterBlend, bool rotate)
{
    QPainter painter;
    painter.begin(m_doubleBuffer);
    if(!hitBlend && waterBlend)
    {
	QImage first = icon.convertToImage();
	QImage second = seaPng.convertToImage();
	painter.drawPixmap(m_x, m_y, seaPng);
	if(rotate)
	    painter.drawImage(m_x, m_y, KImageEffect::blend(first, second, KImageEffect::VerticalGradient, 80, 80));
	else
	    painter.drawImage(m_x, m_y, KImageEffect::blend(first, second, KImageEffect::HorizontalGradient, 80, 80));

    }
    else if(hitBlend && !waterBlend)
    {
	painter.drawPixmap(m_x, m_y, icon);
	painter.drawPixmap(m_x, m_y, hitPng);
    }
    else
	painter.drawPixmap(m_x, m_y, icon);
    
    if(!m_drawGrid)
	painter.end();
    else
    {
	painter.setBrush(NoBrush);
	painter.setPen(black);
	painter.drawRect(m_x, m_y, m_size, m_size);
	painter.end();
    }
}

QString KGridWidget::findIcon(const QString &name)
{
    return locate("data", "kbattleship/pictures/" + name);
}

void KGridWidget::finished()
{
    bitBlt(static_cast<QWidget *>(parent()), 0, 0, m_doubleBuffer);
    cleanBuffer();
}

void KGridWidget::cleanBuffer()
{
    m_doubleBuffer->fill(QApplication::palette().color(QPalette::Normal, QColorGroup::Background));
}
