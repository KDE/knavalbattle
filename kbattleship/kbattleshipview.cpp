/***************************************************************************
                             kbattleshipview.cpp
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

#include "kbattleshipview.moc"

KBattleshipView::KBattleshipView( QWidget *parent, const char *name ) : QWidget( parent, name )
{
}

KBattleshipView::~KBattleshipView()
{
}

void KBattleshipView::startDrawing()
{
    setMinimumSize( 600, 300 );
    setBackgroundMode( PaletteBase );
    QPainter ownpainter( this );
    QPainter enemypainter( this );
    KBattleFieldType owntype;
    KBattleFieldType enemytype;
    owntype.setType( KBattleFieldType::OWNFIELD );
    enemytype.setType( KBattleFieldType::ENEMYFIELD );
    ownfield = new KBattleField( this, owntype, &ownpainter );
    enemyfield = new KBattleField( this, enemytype, &enemypainter );

    connect( ownfield, SIGNAL( doOwnFieldShipListJob( int, int, QPainter *, bool, bool ) ), this, SIGNAL( requestedOwnFieldShipListJob( int, int, QPainter *, bool, bool ) ) );
    connect( enemyfield, SIGNAL( doEnemyFieldShipListJob( int, int, QPainter * ) ), this, SIGNAL( requestedEnemyFieldShipListJob( int, int, QPainter * ) ) );
}

void KBattleshipView::clearField()
{
    ownfield->clearField();
    enemyfield->clearField();
    paintOwnField();
    paintEnemyField();
}

void KBattleshipView::giveOwnFieldShipListType( QPainter *painter, int type, bool hit, bool death )
{
    ownfield->requestedShipIconDraw( painter, type, hit, death );
}

void KBattleshipView::giveEnemyFieldShipListType( QPainter *painter, int type )
{
    enemyfield->requestedShipIconDraw( painter, type );
}

int KBattleshipView::getOwnFieldState( int &fieldx, int &fieldy )
{
    return ownfield->getState( fieldx, fieldy );
}

int KBattleshipView::getEnemyFieldState( int &fieldx, int &fieldy )
{
    return enemyfield->getState( fieldx, fieldy );
}

void KBattleshipView::changeOwnFieldData( int fieldx, int fieldy, int type )
{
    ownfield->changeData( fieldx, fieldy, type );
    paintOwnField();
}

void KBattleshipView::changeEnemyFieldData( int fieldx, int fieldy, int type )
{
    enemyfield->changeData( fieldx, fieldy, type );
    paintEnemyField();
}

void KBattleshipView::mouseReleaseEvent( QMouseEvent *event )
{
    int fieldX, fieldY, fieldTopPos, fieldBottomPos, fieldLeftPos, fieldRightPos, i, j;
    fieldX = 0;
    fieldY = 0;
    if( event->x() <= ( width() / 2 ) - 15 && event->x() >= 46 ) 
    {
	if( event->y() >= 35 && event->y() <= ( height() / 2 ) + 75 )
	{
	    fieldTopPos = 25;
	    fieldBottomPos = ( height() / 2 ) + 75;
	    
	    fieldLeftPos = 25;
	    fieldRightPos = ( width() / 2 ) + 75;
	    
	    i = 0;
	    j = 0;
	    
	    for( i = fieldLeftPos; i <= fieldRightPos; i += 30 )
	    {
		j++;
		if( event->x() >= i - 50 && event->x() <= i + 50 )
		{
		    fieldX = j - 1;
		    break;
		}
	    }

	    i = 0;
	    j = 0;
	
	    for( i = fieldTopPos; i <= fieldBottomPos; i += 30 )
	    {
		j++;
		if( event->y() >= i - 30 && event->y() <= i + 30 )
		{
		    fieldY = j - 1;
		    break;
		}
	    }
	    	    
	    emit ownFieldClicked( fieldX, fieldY, event->button() );
	}
    }

    if( event->x() >= ( width() / 2 ) + 16 && event->x() <= width() - 46 )
    {
    	if( event->y() >= 35 && event->y() <= ( height() / 2 ) + 75 )
	{
	    fieldTopPos = 25;
	    fieldBottomPos = ( height() / 2 ) + 75;
	    
	    fieldLeftPos = 25;
	    fieldRightPos = ( width() / 2 ) + 75;

	    i = 0;
	    j = 0;
	    
	    for( i = fieldLeftPos; i <= fieldRightPos; i += 30 )
	    {
		j++;
		if( event->x() >= i + ( height() / 2 ) - 30 && event->x() <= i + ( height() / 2 ) + 30 )
		{
		    fieldX = j - 4;
		    break;
		}
	    }
        	
	    i = 0;
	    j = 0;
	
	    for( i = fieldTopPos; i <= fieldBottomPos; i += 30 )
	    {
		j++;
		if( event->y() >= i - 30 && event->y() <= i + 30 )
		{
		    fieldY = j - 1;
		    break;
		}		
	    }
	    
	    emit enemyFieldClicked( fieldX, fieldY );
	}
    }
}

void KBattleshipView::paintEnemyField()
{
    QPainter enemypainter( this );
    enemyfield->drawField( &enemypainter );
}

void KBattleshipView::paintOwnField()
{
    QPainter ownpainter( this );
    ownfield->drawField( &ownpainter );
}

void KBattleshipView::paintEvent( QPaintEvent * )
{
    QPainter ownpainter( this );
    QPainter enemypainter( this );
    ownfield->drawField( &ownpainter );
    enemyfield->drawField( &enemypainter );
}

