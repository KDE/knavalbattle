/***************************************************************************
                                kgridwidget.h
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

#ifndef KGRIDWIDGET_H
#define KGRIDWIDGET_H

#include <qpixmap.h>

class KGridWidget
{
public:
	KGridWidget(QWidget *parent, bool draw);
	~KGridWidget();

	void enableGrid() { m_drawGrid = true; }
	void disableGrid() { m_drawGrid = false; }

protected:
	void cleanBuffer();
	void finished();
	void setValues(int x, int y, int size);
	void drawSquare();
	void drawWaterIcon();
	void drawDeathBorder();
	void drawDeathIcon();
	void drawHitIcon();
	void drawShipIcon(int type, bool rotate = false, bool hit = false, bool water = false);
	void drawShipIcon(int ship, int part, bool rotate = false, bool hit = false);

private:
	void cacheImages();
	void drawIcon(const QPixmap &icon, bool hitBlend = false, bool waterBlend = false, bool rotate = false);
	QString findIcon(const QString &name) const;

	bool m_drawGrid;
	int m_x, m_y, m_size;
	QPixmap *m_doubleBuffer;
	QPixmap seaPng, waterPng, hitPng, borderPng,deathPng;
	QPixmap ship1p1Png, ship1p1rPng;
	QPixmap ship2p1Png, ship2p1rPng;
	QPixmap ship2p2Png, ship2p2rPng;
	QPixmap ship3p1Png, ship3p1rPng;
	QPixmap ship3p2Png, ship3p2rPng;
	QPixmap ship3p3Png, ship3p3rPng;
	QPixmap ship4p1Png, ship4p1rPng;
	QPixmap ship4p2Png, ship4p2rPng;
	QPixmap ship4p3Png, ship4p3rPng;
	QPixmap ship4p4Png, ship4p4rPng;
	QWidget *m_parent;
};

#endif
