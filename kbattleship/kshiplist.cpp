/***************************************************************************
                                kshiplist.cpp
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

#include "kshiplist.moc"

KShipList::KShipList()
{
    shipsadded = 4;
    shiplist.setAutoDelete( true );
}

KShipList::~KShipList()
{
}

int KShipList::getXYShipType( int x, int y )
{
    int tempx, tempy;
    KShip *shipiterator;
    for( shipiterator = shiplist.first(); shipiterator != 0; shipiterator = shiplist.next() )
    {
	for( tempy = shipiterator->shipystart(); tempy <= shipiterator->shipystop(); tempy++ )
	{
	    if( tempy == y )	
	    {
		for( tempx = shipiterator->shipxstart(); tempx <= shipiterator->shipxstop(); tempx++ )
		{
		    if( tempx == x )	
			return shipiterator->shiptype();
		}
	    }
        }
    }
    return 99;
}

bool KShipList::canAddShips()
{
    if( shipsadded >= 1)
	return true;
    else
	return false;
}

int KShipList::returnX1ship( int ship )
{
    KShip *shipiterator;
    int i = 0;
    for( shipiterator = shiplist.first(); shipiterator != 0; shipiterator = shiplist.next() )
    {
	i++;
	if( i == ship )
	    return shipiterator->shipxstart();
    }
    return 0;
}

int KShipList::returnX2ship( int ship )
{
    KShip *shipiterator;
    int i = 0;
    for( shipiterator = shiplist.first(); shipiterator != 0; shipiterator = shiplist.next() )
    {
	i++;
	if( i == ship )
	    return shipiterator->shipxstop();
    }
    return 0;
}

int KShipList::returnY1ship( int ship )
{
    KShip *shipiterator;
    int i = 0;
    for( shipiterator = shiplist.first(); shipiterator != 0; shipiterator = shiplist.next() )
    {
	i++;
	if( i == ship )
	    return shipiterator->shipystart();
    }
    return 0;
}

int KShipList::returnY2ship( int ship )
{
    KShip *shipiterator;
    int i = 0;
    for( shipiterator = shiplist.first(); shipiterator != 0; shipiterator = shiplist.next() )
    {
	i++;
	if( i == ship )
	    return shipiterator->shipystop();
    }
    return 0;
}

void KShipList::addShip1( int fieldx1, int fieldx2, int fieldy1, int fieldy2 )
{
    shiplist.append( new KShip( fieldx1, fieldx2, fieldy1, fieldy2, 3 ) );
}

void KShipList::addShip2( int fieldx1, int fieldx2, int fieldy1, int fieldy2 )
{
    shiplist.append( new KShip( fieldx1, fieldx2, fieldy1, fieldy2, 2 ) );
}

void KShipList::addShip3( int fieldx1, int fieldx2, int fieldy1, int fieldy2 )
{
    shiplist.append( new KShip( fieldx1, fieldx2, fieldy1, fieldy2, 1 ) );
}

void KShipList::addShip4( int fieldx1, int fieldx2, int fieldy1, int fieldy2 )
{
    shiplist.append( new KShip( fieldx1, fieldx2, fieldy1, fieldy2, 0 ) );
}

void KShipList::addNewShip( int button, int fieldx, int fieldy )
{
    KShip *shipiterator;
    int tempx, tempy, tempx2, tempy2;
    bool xokay = true;
    bool yokay = true;

    if( shipCount() == 4 )
    {
    	xokay = true;
	yokay = true;
    }
    else
    {
	for( shipiterator = shiplist.first(); shipiterator != 0; shipiterator = shiplist.next() )
	{
	    if( shipiterator->shipystart() == shipiterator->shipystop() && shipiterator->shipxstart() == shipiterator->shipxstop() )
	    {
		for( tempx = shipiterator->shipxstart(); tempx <= shipiterator->shipxstop(); tempx++ )
		{
		    if( tempx == fieldx )
			xokay = false;
		}
		
		for( tempy = shipiterator->shipystart(); tempy <= shipiterator->shipystop(); tempy++ )
		{
		    if( tempy == fieldy )
			yokay = false;
		}
	    }	
	    else if( shipiterator->shipystart() == shipiterator->shipystop() && shipiterator->shipxstart() != shipiterator->shipxstop() )
	    {
		for( tempx = shipiterator->shipxstart(); tempx <= shipiterator->shipxstop(); tempx++ )
		{
		    for( tempx2 = fieldx; tempx2 <= ( fieldx + shipCount() ); tempx2++ )
		    {
			if( tempx == tempx2 )
			    xokay = false;    
		    }
		}
	    }
	    else if( shipiterator->shipxstart() == shipiterator->shipxstop() && shipiterator->shipystart() != shipiterator->shipystop() )
	    {
		for( tempy = shipiterator->shipystart(); tempy <= shipiterator->shipystop(); tempy++ )
		{
		    for( tempy2 = fieldy; tempy2 <= ( fieldy + shipCount() ); tempy2++ )
		    {
			if( tempy == tempy2 )
			    yokay = false;    
		    }
		}
	    }

	    int neighbourp, neighbourm;
	    if( shipCount() != 1 )
		neighbourp = getXYShipType( fieldx, fieldy + shipCount() + 1 );
	    else
		neighbourp = getXYShipType( fieldx, fieldy + shipCount() );
		
	    neighbourm = getXYShipType( fieldx, fieldy - 1 );
		
	    if( neighbourp != 99 && neighbourm != 99 )
	    {
	        xokay = false;
		yokay = false;
	        break;
	    }

	    if( neighbourp != 99 && neighbourm == 99 )
	    {
	        xokay = false;
		yokay = false;
	        break;
	    }
		
	    if( neighbourp == 99 && neighbourm != 99 )
	    {
		xokay = false;
		yokay = false;
	        break;
	    }
	    
	    if( shipCount() != 1 )
		neighbourp = getXYShipType( fieldx + shipCount() + 1, fieldy );
	    else
		neighbourp = getXYShipType( fieldx + shipCount(), fieldy );
		
	    neighbourm = getXYShipType( fieldx - 1, fieldy );

	    if( neighbourp != 99 && neighbourm != 99 )
	    {
	        xokay = false;
		yokay = false;
	        break;
	    }

	    if( neighbourp != 99 && neighbourm == 99 )
	    {
	        xokay = false;
		yokay = false;
	        break;
	    }
		
	    if( neighbourp == 99 && neighbourm != 99 )
	    {
		xokay = false;
		yokay = false;
	        break;
	    }
	    
	    if( xokay && !yokay )
	    {
		xokay = true;
		yokay = true;
	    }
	    
	    if( yokay && !xokay )
	    {
		xokay = true;
		yokay = true;
	    }
	}
    }
    
    if( xokay && yokay )
    {
	shipsadded--;
	switch( button )
	{
	    case LeftButton:
		shiplist.append( new KShip( fieldx, fieldx + shipCount(), fieldy, fieldy, shipCount() ) );
		break;

	    case RightButton:
	    	shiplist.append( new KShip( fieldx, fieldx, fieldy, fieldy + shipCount(), shipCount() ) );
		break;
	}
     	decideShipPlacing( button, fieldx, fieldy );
	if( shipsadded == 0 )
	    emit lastShipAdded();
	return;
    }
	
    if( xokay && !yokay )
    {
	shipsadded--;
    	switch( button )
	{
	    case LeftButton:
		shiplist.append( new KShip( fieldx, fieldx + shipCount(), fieldy, fieldy, shipCount() ) );
		break;

	    case RightButton:
	    	shiplist.append( new KShip( fieldx, fieldx, fieldy, fieldy + shipCount(), shipCount() ) );
		break;
	}
     	decideShipPlacing( button, fieldx, fieldy );
	if( shipsadded == 0 )
	    emit lastShipAdded();
	return;
    }

    if( !xokay && yokay )
    {
    	shipsadded--;
	switch( button )
	{
		case LeftButton:
	    	    shiplist.append( new KShip( fieldx, fieldx + shipCount(), fieldy, fieldy, shipCount() ) );
		    break;

		case RightButton:
		    shiplist.append( new KShip( fieldx, fieldx, fieldy, fieldy + shipCount(), shipCount() ) );
		    break;
	}
     	decideShipPlacing( button, fieldx, fieldy );
	if( shipsadded == 0 )
	    emit lastShipAdded();
	return;
    }

    if( !xokay && !yokay )
    {
	kdDebug() << "CAN'T PLACE!" << endl;
	return;
    }
}

int KShipList::shipCount()
{
    return shipsadded;
}

void KShipList::decideShipPlacing( int button, int fieldx, int fieldy )
{
    switch( button )
    {
	case LeftButton:
	    if( fieldx + shipCount() <= 8 )
		placeShipLMB( fieldx, fieldy );
	    break;

	case RightButton:
	    if( fieldy + shipCount() <= 8 )
		placeShipRMB( fieldx, fieldy );
	    break;
    }
}

void KShipList::placeShipLMB( int fieldx, int fieldy )
{ 	
    switch( shipCount() )
    {
	case 3:
	    controlOwnFieldData( fieldx, fieldy, KBattleField::SHIP );
	    controlOwnFieldData( fieldx + 1, fieldy, KBattleField::SHIP );
	    controlOwnFieldData( fieldx + 2, fieldy, KBattleField::SHIP );
	    controlOwnFieldData( fieldx + 3, fieldy, KBattleField::SHIP );
	    break;
			
	case 2:
	    controlOwnFieldData( fieldx, fieldy, KBattleField::SHIP );
	    controlOwnFieldData( fieldx + 1, fieldy, KBattleField::SHIP );
	    controlOwnFieldData( fieldx + 2, fieldy, KBattleField::SHIP );
	    break;

	case 1:
	    controlOwnFieldData( fieldx, fieldy, KBattleField::SHIP );
	    controlOwnFieldData( fieldx + 1, fieldy, KBattleField::SHIP );
	    break;

	case 0:
	    controlOwnFieldData( fieldx, fieldy, KBattleField::SHIP );
	    break;
    }	
}

void KShipList::placeShipRMB( int fieldx, int fieldy )
{ 	
    switch( shipCount() )
    {
	case 3:
	    controlOwnFieldData( fieldx, fieldy, KBattleField::SHIP );
	    controlOwnFieldData( fieldx, fieldy + 1, KBattleField::SHIP );
	    controlOwnFieldData( fieldx, fieldy + 2, KBattleField::SHIP );
	    controlOwnFieldData( fieldx, fieldy + 3, KBattleField::SHIP );
	    break;
			
	case 2:
	    controlOwnFieldData( fieldx, fieldy, KBattleField::SHIP );
	    controlOwnFieldData( fieldx, fieldy + 1, KBattleField::SHIP );
	    controlOwnFieldData( fieldx, fieldy + 2, KBattleField::SHIP );
	    break;

	case 1:
	    controlOwnFieldData( fieldx, fieldy, KBattleField::SHIP );
	    controlOwnFieldData( fieldx, fieldy + 1, KBattleField::SHIP );
	    break;

	case 0:
	    controlOwnFieldData( fieldx, fieldy, KBattleField::SHIP );
	    break;
    }	
}

void KShipList::controlOwnFieldData( int fieldx, int fieldy, int type )
{
    emit ownFieldDataChanged( fieldx, fieldy, type );
}
