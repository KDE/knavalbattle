/***************************************************************************
                               kbattlefield.cpp
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

#include "kbattlefield.moc"

KBattleField::KBattleField( QWidget *parent, const KBattleFieldType &type ) : KGridWidget( parent )
{

    internalType = type.getType();
    kdDebug() << "Type: " << internalType;
    int i, j, it, jt;

    // Hehe this is quite unusual now
    // I don't have any Battleship rulesets
    // here so this is wrong, i think (WildFox)
    
    for( i = 0; i != 8; i++ ) // A to I
    {
	for( j = 0; j != 8; j++ ) // 1 to 9
	{
	    FieldData[ i ][ j ] = KBattleField::FREE;	     
	}
    }
    
    // Drawing
    setDrawValues( parent );
    
    kdDebug() << "LeftV: " << LeftV << endl;
    kdDebug() << "RightV: " << RightV << endl;
    kdDebug() << "TopV: " << TopV << endl;
    kdDebug() << "BottomV: " << BottomV << endl;
    
    for( it = TopV; it <= BottomV; it +=40 )
    {
	for( jt = LeftV; jt <= RightV; jt +=40 )
	{
	    kdDebug() << "it: " << it << " jt: " << jt << endl;
	    setValues( it, jt, 40 );	    
	    drawSquare( parent );
	}
    }
    
}

KBattleField::~KBattleField()
{
}

void KBattleField::setDrawValues( QWidget *parent )
{
    kdDebug() << "Width: " << parent->width() << endl;
    kdDebug() << "Height: " << parent->height() << endl;
    
    switch( internalType )
    {
	case KBattleFieldType::OWNFIELD:
	{
	    kdDebug() << "OWNFIELD" << endl;
	    LeftV = 40;
	    RightV = ( parent->width() / 2 ) - 20;
	    break;
	}
	
	case KBattleFieldType::ENEMYFIELD:
	{
	    kdDebug() << "ENEMYFIELD" << endl;
	    LeftV = ( parent->width() / 2 ) + 20;
	    RightV = parent->width() - 40;
	    break;
	}
    }
    
    BottomV = parent->height() - 40;
    TopV = 40;
}

void KBattleField::fieldResized()
{
    // TODO: add this later
}
