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
#include <qlist.h>
#include <qwidget.h>
#include "kbattlefieldtype.h"
#include "kgridwidget.h"

class KBattleField : public KGridWidget
{
    Q_OBJECT
    public:
		enum{ WATER = 90, HIT = 91, DEATH = 92, SHIP = 93 };
		KBattleField();
        KBattleField( QWidget *parent, const KBattleFieldType &type, QPainter *painter );
        ~KBattleField();
	
		void requestedShipIconDraw( QPainter *painter, int type );
		void setDrawValues( QWidget *parent );
		void drawField( QPainter *painter );
		void clearField();
		void changeData( int &fieldx, int &fieldy, int type );
		int findOwnFieldShipType( int x, int y, QPainter *painter );
		int findEnemyFieldShipType( int x, int y, QPainter *painter );
		int getState( int fieldx, int fieldy );

    private:
		int FieldData[ 8 ][ 8 ];
		int internalType;
		int FromLeft;

	signals:
		void doOwnFieldShipListJob( int fieldx, int fieldy, QPainter *painter );
		void doEnemyFieldShipListJob( int fieldx, int fieldy, QPainter *painter );

};

#endif
