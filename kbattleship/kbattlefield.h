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
        enum{OWNFIELD, ENEMYFIELD, FREE, WATER, HIT, DEATH, SHIP};
        KBattleField(QWidget *parentw, const char *name, int type);
        ~KBattleField();
    
        void drawField();
        void clearField();
        void changeData(int &fieldx, int &fieldy, int type) { FieldData[fieldx][fieldy] = type; }
        int getState(int fieldx, int fieldy) { return FieldData[fieldx][fieldy]; }

	QWidget *drawParent() { return m_parent_widget; }

	QRect getRect();

    private:
	int xPosition();
        int FieldData[8][8];
        int m_type;
	QWidget *m_parent_widget;
};

#endif
