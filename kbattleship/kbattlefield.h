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
#include <qwidget.h>

#include "kgridwidget.h"

class KBattleField : public KGridWidget
{
    Q_OBJECT
    public:
        enum{FREE, WATER, HIT, DEATH, SHIP1P1, SHIP2P1, SHIP2P2, SHIP3P1, SHIP3P2, SHIP3P3, SHIP4P1, SHIP4P2, SHIP4P3, SHIP4P4};
        KBattleField(QWidget *parentw, const char *name);
        ~KBattleField();
    
        void drawOwnField();
        void drawEnemyField();
        void clearOwnField();
	void clearEnemyField();

	void setDrawField(bool draw) { m_canDraw = draw; }
	
        void changeOwnData(int &fieldx, int &fieldy, int type) { m_ownfield[fieldx][fieldy] = type; }
        int getOwnState(int fieldx, int fieldy) { return m_ownfield[fieldx][fieldy]; }

        void changeEnemyData(int &fieldx, int &fieldy, int type) { m_enemyfield[fieldx][fieldy] = type; }
        int getEnemyState(int fieldx, int fieldy) { return m_enemyfield[fieldx][fieldy]; }

	QWidget *drawParent() { return m_parent_widget; }

	QRect getOwnRect();
	QRect getEnemyRect();
	
	int gridSize() { return 30; }

    private:
    	int ownXPosition();
	int enemyXPosition();
	int rectX();
	
        int m_ownfield[15][15];
        int m_enemyfield[15][15];

	int m_ownfieldx;
	int m_ownfieldy;
	int m_enemyfieldx;
	int m_enemyfieldy;
		
	int m_width;
	
	bool m_canDraw;

	QWidget *m_parent_widget;
};

#endif
