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
    QWhatsThis::add( this, "foo"  );
    int i, j;

    // Hehe this is quite unusual now
    // I don't have any Battleship rulesets
    // here so this is wrong, i think (WildFox)
    
    for( i = 0; i != 8; i++ ) // A to I
    {
	for( j = 0; j != 8; j++ ) // 1 to 9
	{
	    FieldData[ i ][ j ] = KBattleField::WATER;     
	}
    }
    

    setDrawValues( this );
    drawField( painter );    
}

KBattleField::~KBattleField()
{
}

void KBattleField::changeData( int &fieldx, int &fieldy, int type )
{
    FieldData[ fieldx ][ fieldy ] = type;
}

int KBattleField::getState( int fieldx, int fieldy )
{
    return FieldData[ fieldx][ fieldy ];
}

void KBattleField::drawField( QPainter *painter )
{
    int i, j;

    for( i = 0; i != 8; i++ ) // A to I
    {
	for( j = 0; j != 8; j++ ) // 1 to 9
	{
	    setValues( ( ( ( i + 1 ) * 30 ) + FromLeft ), ( ( ( j + 1 ) * 30 ) + 5 ) , 30 );
	    drawSquare( painter );	    
	    switch( FieldData[ i ][ j ] )
	    {
		case KBattleField::WATER:
		    break;
		
		case KBattleField::HIT:
		    drawHitIcon( painter );
		    break;
		    
		case KBattleField::DEATH:
		    drawDeathIcon( painter );
		    break;	

		case KBattleField::SHIP1P1:
		    drawShipIcon( painter, 1, 1 );
		    break;	    
		    
		case KBattleField::SHIP2P1:
		    drawShipIcon( painter, 2, 1 );
		    break;	    
		    
		case KBattleField::SHIP2P2:
		    drawShipIcon( painter, 2, 2 );
		    break;	    
		    
		case KBattleField::SHIP3P1:
		    drawShipIcon( painter, 3, 1 );
		    break;	    
		    
		case KBattleField::SHIP3P2:
		    drawShipIcon( painter, 3, 2 );
		    break;	    

		case KBattleField::SHIP3P3:
		    drawShipIcon( painter, 3, 3 );
		    break;	    

		case KBattleField::SHIP4P1:
		    drawShipIcon( painter, 4, 1 );
		    break;	    
		    
		case KBattleField::SHIP4P2:
		    drawShipIcon( painter, 4, 2 );
		    break;	    
		    
		case KBattleField::SHIP4P3:
		    drawShipIcon( painter, 4, 3 );
		    break;	    

		case KBattleField::SHIP4P4:
		    drawShipIcon( painter, 4, 4 );
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
