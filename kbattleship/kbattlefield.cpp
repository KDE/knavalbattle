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

KBattleField::KBattleField( QWidget *parent, const char *name, int type ) : KGridWidget( parent, name )
{
    internalType = type;
    int i, j;

    for( i = 0; i != 8; i++ )
    {
        for( j = 0; j != 8; j++ )
        {
            FieldData[ i ][ j ] = KBattleField::FREE;
    	}
    }

    setDrawValues();
    drawField();
}

KBattleField::~KBattleField()
{
}

void KBattleField::clearField()
{
    int i, j;
    for( i = 0; i != 8; i++ )
    {
        for( j = 0; j != 8; j++ )
    	{
	        FieldData[ i ][ j ] = KBattleField::FREE;
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

void KBattleField::drawField()
{
    int i, j;

    QPainter painter ( static_cast<QWidget *>( parent() ) );
    for( i = 0; i != 8; i++ )
    {
        for( j = 0; j != 8; j++ )
        {
            setValues( ( ( ( i + 1 ) * 30 ) + FromLeft ), ( ( ( j + 1 ) * 30 ) + 5 ) , 30 );
            switch( FieldData[ i ][ j ] )
	        {
		        case KBattleField::FREE:
                    drawSquare( &painter );	
                    break;
		
		        case KBattleField::WATER:
		            drawSquare( &painter );	
		            drawWaterIcon( &painter );
		            break;
		
		        case KBattleField::HIT:
		            drawSquare( &painter );	
		            if( internalType == KBattleField::OWNFIELD )
			            findOwnFieldShipType( i, j, true );
                    else if( internalType == KBattleField::ENEMYFIELD )
			            drawHitIcon( &painter );
		            break;
		    
		        case KBattleField::DEATH:
		            drawSquare( &painter );	
		            drawDeathIcon( &painter );
		            break;	

		        case KBattleField::SHIP:
		            drawSquare( &painter );	
		            if( internalType == KBattleField::OWNFIELD )
                        findOwnFieldShipType( i, j );
                    else if( internalType == KBattleField::ENEMYFIELD )
                        findEnemyFieldShipType( i, j );
                    break;
            }
        }		
    }
    painter.end();
}

void KBattleField::requestedShipIconDraw( int type, bool hit, bool death )
{
    QPainter painter ( static_cast<QWidget *>( parent() ) );
    drawShipIcon( &painter, type );
    if( hit )
	    drawHitIcon( &painter );
	else if( death )
	    drawDeathIcon( &painter );

    painter.end();
}

void KBattleField::findOwnFieldShipType( int x, int y, bool hit, bool death )
{
    emit doOwnFieldShipListJob( x, y, hit, death ); 	
}

void KBattleField::findEnemyFieldShipType( int x, int y )
{
    emit doEnemyFieldShipListJob( x, y ); 	
}

void KBattleField::setDrawValues()
{
    if( internalType == KBattleField::OWNFIELD )
        FromLeft = 15;
    else if( internalType == KBattleField::ENEMYFIELD )
        FromLeft = ( static_cast<QWidget *>( parent() )->width() / 2 );
}
