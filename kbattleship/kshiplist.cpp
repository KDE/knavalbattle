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
					{
						return shipiterator->shiptype();
					}
				}
			}
		}
	}
}

bool KShipList::canAddShips()
{
    if( shipsadded >= 1)
	{
		return true;
	}
    else
	{
		return false;
	}
}

void KShipList::addNewShip( int button, int fieldx, int fieldy )
{
	KShip *shipiterator;
	int tempx, tempy;
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
			for( tempy = shipiterator->shipystart(); tempy <= shipiterator->shipystop(); tempy++ )
			{
				if( tempy == fieldy )	
				{
					yokay = false;
				}
			}
	
			for( tempx = shipiterator->shipxstart(); tempx <= shipiterator->shipxstop(); tempx++ )
			{
				if( tempx == fieldx )	
				{
					xokay = false;
				}
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
		    {
            	placeShipLMB( fieldx, fieldy );
		    }
			break;

		case RightButton:
			if( fieldy + shipCount() <= 8 )
		    {
				placeShipRMB( fieldx, fieldy );
	    	}
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
