/***************************************************************************
                                kbattlefield.h
                             -------------------
    Developers: (c) 2000 Nikolas Zimmermann <wildfox@kde.org>
                (c) 2000 Daniel Molkentin <molkentin@kde.org>

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

#include <kdebug.h>
#include <qpainter.h>
#include <qwidget.h>
#include <qwhatsthis.h>
#include "kbattlefieldtype.h"
#include "kgridwidget.h"

class KBattleField : public KGridWidget
{
    Q_OBJECT
    public:
	enum{ WATER, HIT, DEATH, SHIP1P1, SHIP2P1, SHIP2P2, SHIP3P1, SHIP3P2, SHIP3P3, SHIP4P1, SHIP4P2, SHIP4P3, SHIP4P4 };
        KBattleField( QWidget *parent, const KBattleFieldType &type, QPainter *painter );
        ~KBattleField();
	
	void setDrawValues( QWidget *parent );
	void drawField( QPainter *painter );
	void changeData( int &fieldx, int &fieldy, int type );
	int getState( int fieldx, int fieldy );

    private:
	int FieldData[ 8 ][ 8 ];
	int internalType;
	int FromLeft;

};

#endif
