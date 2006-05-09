/***************************************************************************
                               kbattlefield.h
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

#ifndef KBATTLEFIELD_H
#define KBATTLEFIELD_H

#include <qpainter.h>
#include <QWidget>

#include "kgridwidget.h"

class KBattleField : public KGridWidget
{
public:
	enum{FREE, WATER, HIT, DEATH, BORDER, SHIP1P1, SHIP2P1, SHIP2P2, SHIP3P1, SHIP3P2, SHIP3P3, SHIP4P1, SHIP4P2, SHIP4P3, SHIP4P4};
	KBattleField(QWidget *parent, bool grid);

	void clearOwnField();
	void clearEnemyField();
	void clearPreviewField();

	void drawField(QPainter &p);
	void setDrawField(bool draw) { m_canDraw = draw; }

 	void setOwnState(int fieldx, int fieldy, int type) { m_ownfield[fieldx][fieldy] = type; }
	int ownState(int fieldx, int fieldy) { return m_ownfield[fieldx][fieldy]; }

	void setEnemyState(int fieldx, int fieldy, int type) { m_enemyfield[fieldx][fieldy] = type; }
	int enemyState(int fieldx, int fieldy) { return m_enemyfield[fieldx][fieldy]; }

	void setPreviewState(int fieldx, int fieldy, int type, bool rotate);

	QRect ownRect();
	QRect enemyRect();

	int gridSize() { return 32; }

private:
	void drawOwnField();
	void drawEnemyField();

	int ownXPosition();
	int ownYPosition();
	int enemyXPosition();
	int enemyYPosition();

	int rectX();

	int m_ownfield[15][15];
	int m_enemyfield[15][15];

	int m_newfield[15][15];
	bool m_newdata[15][15];
	bool m_rotatedata[15][15];

	int m_ownfieldx;
	int m_ownfieldy;
	int m_enemyfieldx;
	int m_enemyfieldy;

	int m_width;

	bool m_canDraw;

	QWidget *m_parent;
};

#endif
