/***************************************************************************
                              kbattleshipview.h
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

#ifndef KBATTLESHIPVIEW_H
#define KBATTLESHIPVIEW_H

#include <qpainter.h>
#include <qwidget.h>
#include "kbattlefieldtype.h"
#include "kbattlefield.h"
#include "kbattleship.h"

class KBattleshipView : public QWidget
{
    Q_OBJECT
    public:
        KBattleshipView( QWidget *parent = 0, const char *name = 0 );
        ~KBattleshipView();

	void startDrawing();
	void changeOwnFieldData( int fieldx, int fieldy, int type );
	void changeEnemyFieldData( int fieldx, int fieldy, int type );
	void paintEnemyField( int fieldx, int fieldy );
	
    protected:
	void paintEvent( QPaintEvent * );
	void mouseReleaseEvent( QMouseEvent *event );
	
    private:
        KBattleField *ownfield;
        KBattleField *enemyfield;
	
    signals:
	void enemyFieldClicked( int fieldX, int fieldY );
};

#endif
