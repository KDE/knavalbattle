/***************************************************************************
                                kbattlefield.h
                             -------------------
    Developers: (c) 2000 Nikolas Zimmermann <nikoz@gymnasium-kerpen.de>
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
#include "kbattlefieldtype.h"
#include "kgridwidget.h"

class KBattleField : public KGridWidget
{
    Q_OBJECT
    public:
	enum { FREE, RESERVED, FIRE, DEATH };
        KBattleField( QWidget *parent, const KBattleFieldType &type, QPainter *painter );
        ~KBattleField();
	
	void setDrawValues( QWidget *parent );
	void drawField( QPainter *painter );
	int LeftV;
	int RightV;    
	int TopV;
	int BottomV;

    private:
	int FieldData[ 8 ][ 8 ];
	int internalType;

};

#endif
