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

KBattleField::KBattleField()
{
}

KBattleField::KBattleField( QWidget *parent, const KBattleFieldType &type, QPainter *painter )
{
    internalType = type.getType();
    int i, j;

    // Hehe this is quite unusual now
    // I don't have any Battleship rulesets
    // here...so this is wrong, i think (WildFox)
    
    for( i = 0; i != 8; i++ ) // A to I
    {
    	for( j = 0; j != 8; j++ ) // 1 to 9
	{
	    switch( internalType )
    	    {
		case KBattleFieldType::OWNFIELD:
		    FieldData[ i ][ j ] = KBattleField::WATER;
		    break;
		    
		case KBattleFieldType::ENEMYFIELD:
		    FieldData[ i ][ j ] = KBattleField::FREE;
		    break;
	    }
    	}
    }

    setDrawValues( this );
    drawField( painter );    
}

KBattleField::~KBattleField()
{
}

void KBattleField::clearField()
{
    int i, j;
    for( i = 0; i != 8; i++ ) // A to I
    {
	for( j = 0; j != 8; j++ ) // 1 to 9
    	{
	    switch( internalType )
    	    {
		case KBattleFieldType::OWNFIELD:
		    FieldData[ i ][ j ] = KBattleField::WATER;
		    break;
		    
		case KBattleFieldType::ENEMYFIELD:
		    FieldData[ i ][ j ] = KBattleField::FREE;
		    break;
	    }
	}
    }
}

void KBattleField::changeData( int &fieldx, int &fieldy, int type )
{
    FieldData[ fieldx ][ fieldy ] = type;
}

int KBattleField::getState( int fieldx, int fieldy )
{
    return FieldData[ fieldx ][ fieldy ];
}

void KBattleField::drawField( QPainter *painter )
{
    int i, j;

    for( i = 0; i != 8; i++ ) // A to I
    {
	for( j = 0; j != 8; j++ ) // 1 to 9
	{
	    setValues( ( ( ( i + 1 ) * 30 ) + FromLeft ), ( ( ( j + 1 ) * 30 ) + 5 ) , 30 );
	    switch( FieldData[ i ][ j ] )
	    {
		case KBattleField::FREE:
		    drawSquare( painter );	
		    break;
		
		case KBattleField::WATER:
		    drawWaterIcon( painter );
		    break;
		
		case KBattleField::HIT:
		    drawSquare( painter );	
		    if( internalType == KBattleFieldType::OWNFIELD )
			findOwnFieldShipType( i, j, painter, true );
		    else
			drawHitIcon( painter );
			
		    break;
		    
		case KBattleField::DEATH:
		    drawSquare( painter );	
		    drawDeathIcon( painter );
		    break;	

		case KBattleField::SHIP:
		    switch( internalType )
    		    {
			case KBattleFieldType::OWNFIELD:
			    findOwnFieldShipType( i, j, painter );
		    	    break;
						
			case KBattleFieldType::ENEMYFIELD:
			    findEnemyFieldShipType( i, j, painter );
		    	    break;
		    }
		    break;
	    }		
	}
    }

    painter->end();    
}

void KBattleField::requestedShipIconDraw( QPainter *painter, int type, bool hit, bool death )
{
    drawShipIcon( painter, type );
    if( hit )
	drawHitIcon( painter );
	
    if( death )
	drawDeathIcon( painter );
}

int KBattleField::findOwnFieldShipType( int x, int y, QPainter *painter, bool hit, bool death )
{
    emit doOwnFieldShipListJob( x, y, painter, hit, death ); 	
}

int KBattleField::findEnemyFieldShipType( int x, int y, QPainter *painter )
{
    emit doEnemyFieldShipListJob( x, y, painter ); 	
}

void KBattleField::setDrawValues( QWidget *parent )
{
    switch( internalType )
    {
	case KBattleFieldType::OWNFIELD:
	    FromLeft = 15;
	    break;

	case KBattleFieldType::ENEMYFIELD:
	    FromLeft = ( parent->width() / 2 ) + 30;
	    break;
    }
}
