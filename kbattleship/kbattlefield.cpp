/***************************************************************************
                               kbattlefield.cpp
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

#include "kbattlefield.moc"

KBattleField::KBattleField( QWidget *parent, const KBattleFieldType &type, QPainter *painter ) : KGridWidget()
{

    internalType = type.getType();
    int i, j;

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
    

    setDrawValues( this );
    drawField( painter );    
}

KBattleField::~KBattleField()
{
}

void KBattleField::changeData( int &fieldX, int &fieldY, int type )
{
    FieldData[ fieldX ][ fieldY ] = type;
}

void KBattleField::drawField( QPainter *painter )
{
    int i, j;

    for( i = 0; i != 8; i++ ) // A to I
    {
	for( j = 0; j != 8; j++ ) // 1 to 9
	{
	    switch( FieldData[ i ][ j ] )
	    {
		case KBattleField::FREE:
		    setValues( ( ( ( i + 1 ) * 30 ) + FromLeft ), ( ( ( j + 1 ) * 30 ) + 5 ) , 30 );
		    drawSquare( painter );	    
		    break;
		
		case KBattleField::WATER:
		    setValues( ( ( ( i + 1 ) * 30 ) + FromLeft ), ( ( ( j + 1 ) * 30 ) + 5 ) , 30 );
		    drawSquare( painter );
		    drawWaterIcon( painter );
		    break;
	    }		    
	}
    }

    painter->end();    
}

void KBattleField::setDrawValues( QWidget *parent )
{
    switch( internalType )
    {
	case KBattleFieldType::OWNFIELD:
	{
	    FromLeft = 15;
	    break;
	}
	
	case KBattleFieldType::ENEMYFIELD:
	{
	    FromLeft = ( parent->width() / 2 ) + 30;
	    break;
	}
    }
    
}


