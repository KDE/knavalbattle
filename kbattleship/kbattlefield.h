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
        enum{OWNFIELD, ENEMYFIELD, FREE, WATER, HIT, DEATH, SHIP1P1, SHIP2P1, SHIP2P2, SHIP3P1, SHIP3P2, SHIP3P3, SHIP4P1, SHIP4P2, SHIP4P3, SHIP4P4};
        KBattleField(QWidget *parentw, const char *name, int type);
        ~KBattleField();
    
        void drawField();
        void clearField();
        void changeData(int &fieldx, int &fieldy, int type) { m_field[fieldx][fieldy] = type; }
        int getState(int fieldx, int fieldy) { return m_field[fieldx][fieldy]; }

	QWidget *drawParent() { return m_parent_widget; }

	QRect getRect();
	
	int gridSize() { return 30; }

    private:
    	int xPosition();
	int rectX();
	
        int m_field[15][15];
        int m_type;

	int m_fieldx;
	int m_fieldy;

	QWidget *m_parent_widget;
};

#endif
