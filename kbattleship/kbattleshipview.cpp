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
    setFixedSize( 600, 400 );
    setBackgroundMode( PaletteBase );

}

KBattleshipView::~KBattleshipView()
{
}

void KBattleshipView::startDrawing()
{
    QPainter ownpainter( this );
    QPainter enemypainter( this );
    KBattleFieldType owntype;
    KBattleFieldType enemytype;
    owntype.setType( KBattleFieldType::OWNFIELD );
    enemytype.setType( KBattleFieldType::ENEMYFIELD );
    ownfield = new KBattleField( this, owntype, &ownpainter );
    enemyfield = new KBattleField( this, enemytype, &enemypainter );
}

void KBattleshipView::mouseReleaseEvent( QMouseEvent *event )
{
    if( event->x() <= ( width() / 2 ) - 15 && event->x() >= 46 ) 
    {
	if( event->y() >= 35 && event->y() <= ( height() / 2 ) + 75 )
	{
	    int fieldX, fieldY, fieldTopPos, fieldBottomPos, fieldLeftPos, fieldRightPos, i, j;
	    
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
	    	    
	    QPainter ownpainter( this );
	    ownfield->changeData( fieldX, fieldY, KBattleField::WATER );
	    ownfield->drawField( &ownpainter );
	}
    }

    if( event->x() >= ( width() / 2 ) + 16 && event->x() <= width() - 46 )
    {
    	if( event->y() >= 35 && event->y() <= ( height() / 2 ) + 75 )
	{
	    int fieldX, fieldY, fieldTopPos, fieldBottomPos, fieldLeftPos, fieldRightPos, i, j;
	    
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

	    QPainter enemypainter( this );
	    enemyfield->changeData( fieldX, fieldY, KBattleField::WATER );
	    enemyfield->drawField( &enemypainter );
	}
    }
    	
//    if( event->x() >= ( width() / 2 ) - 14 && event->x() <= ( width() / 2 ) + 15 )
//    {
//	kdDebug() << "NOFIELD!" << endl;
//    }
	
//    if( event->x() <= 45 || event->x() >= width() - 45 )
//    {
//	kdDebug() << "NOFIELD!" << endl;
//    }
}

void KBattleshipView::paintEvent( QPaintEvent * )
{
    QPainter ownpainter( this );
    QPainter enemypainter( this );
    ownfield->drawField( &ownpainter );
    enemyfield->drawField( &enemypainter );
}

