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

KShipList::KShipList() : QObject()
{
    shiplist.setAutoDelete(true);
    shipsadded = 4;
}

KShipList::~KShipList()
{
}

void KShipList::clear()
{
    shipsadded = 4;
    shiplist.clear();
}

int KShipList::getXYShipType(int x, int y)
{
    kdDebug() << "CHECKING SHIP TYPE FOR X: " << x << " Y: " << y << " ...." << endl;
    int tempx, tempy;
    KShip *shipiterator;
    for(shipiterator = shiplist.first(); shipiterator != 0; shipiterator = shiplist.next())
    {
	if(shipiterator->shipystart() != shipiterator->shipystop())
	{
	    for(tempy = shipiterator->shipystart(); tempy <= shipiterator->shipystop(); tempy++)
	    {
   		if(tempy == y)
		{
		    if(shipiterator->shipxstart() != shipiterator->shipxstop())
		    {
 			for(tempx = shipiterator->shipxstart(); tempx <= shipiterator->shipxstop(); tempx++)
			{
			    if(tempx == x)
		    		return shipiterator->shiptype();
			}
		    }
		    else
		    {
		        tempx = shipiterator->shipxstart();
			if(tempx == x)
		    	    return shipiterator->shiptype();
		    }
		}
	    }
	}
	else
	{
	    tempy = shipiterator->shipystart();
	    if(tempy == y)
	    {
	        if(shipiterator->shipxstart() != shipiterator->shipxstop())
		{
 		    for(tempx = shipiterator->shipxstart(); tempx <= shipiterator->shipxstop(); tempx++)
		    {
			if(tempx == x)
			    return shipiterator->shiptype();
		    }
		}
		else
		{
		    tempx = shipiterator->shipxstart();
		    if(tempx == x)
		        return shipiterator->shiptype();
		}
	    }
	}
    }
    return 99;
}

KShip *KShipList::getXYShip(int x, int y)
{
    int tempx, tempy;
    KShip *shipiterator;
    for(shipiterator = shiplist.first(); shipiterator != 0; shipiterator = shiplist.next())
    {
	for(tempy = shipiterator->shipystart(); tempy <= shipiterator->shipystop(); tempy++)
	{
	    if(tempy == y)
	    {
	        for(tempx = shipiterator->shipxstart(); tempx <= shipiterator->shipxstop(); tempx++)
		{
		    if(tempx == x)
		        return shipiterator;
	        }
	    }
        }
    }
    return 0;
}

bool KShipList::canAddShips()
{
    if(shipsadded >= 1)
	return true;
    else
        return false;
}

KShip *KShipList::returnIterator(int ship)
{
    KShip *shipiterator;
    int i = 0;
    for(shipiterator = shiplist.first(); shipiterator != 0; shipiterator = shiplist.next())
    {
        i++;
        if(i == ship)
	    return shipiterator;
    }
    return 0;
}

int KShipList::returnX1Ship(int ship)
{
    return (returnIterator(ship))->shipxstart();
}

int KShipList::returnX2Ship(int ship)
{
    return (returnIterator(ship))->shipxstop();
}

int KShipList::returnY1Ship(int ship)
{
    return (returnIterator(ship))->shipystart();
}

int KShipList::returnY2Ship(int ship)
{
    return (returnIterator(ship))->shipystop();
}

void KShipList::addShip(int fieldx1, int fieldx2, int fieldy1, int fieldy2, int ship)
{
    shiplist.append(new KShip(fieldx1, fieldx2, fieldy1, fieldy2, ship));
}

void KShipList::addShip1(int fieldx1, int fieldx2, int fieldy1, int fieldy2)
{
    addShip(fieldx1, fieldx2, fieldy1, fieldy2, 3);
}

void KShipList::addShip2(int fieldx1, int fieldx2, int fieldy1, int fieldy2)
{
    addShip(fieldx1, fieldx2, fieldy1, fieldy2, 2);
}

void KShipList::addShip3(int fieldx1, int fieldx2, int fieldy1, int fieldy2)
{
    addShip(fieldx1, fieldx2, fieldy1, fieldy2, 1);
}

void KShipList::addShip4(int fieldx1, int fieldx2, int fieldy1, int fieldy2)
{
    addShip(fieldx1, fieldx2, fieldy1, fieldy2, 0);
}

void KShipList::addNewShip(int button, int fieldx, int fieldy)
{
    KShip *shipiterator;
    int tempx, tempy, tempx2, tempy2;
    bool xokay = true;
    bool yokay = true;

    if(shipCount() != 4)
    {
        for(shipiterator = shiplist.first(); shipiterator != 0; shipiterator = shiplist.next())
        {
            // Check for X and Y for 1 quare ship
	    if( (shipiterator->shipystart() == shipiterator->shipystop() ) &&
                (shipiterator->shipxstart() == shipiterator->shipxstop() ) )
	    {
	        for(tempx = shipiterator->shipxstart(); tempx <= shipiterator->shipxstop(); tempx++)
	        {
	            if(tempx == fieldx)
		        xokay = false;
		}

		for(tempy = shipiterator->shipystart(); tempy <= shipiterator->shipystop(); tempy++)
		{
		    if(tempy == fieldy)
		        yokay = false;
		}

	    }
            // Check for X for ships > 1 square
	    else if( (shipiterator->shipystart() == shipiterator->shipystop() ) &&
                     (shipiterator->shipxstart() != shipiterator->shipxstop() ) )
	    {
	        for(tempx = shipiterator->shipxstart(); tempx <= shipiterator->shipxstop(); tempx++)
	        {
	            for(tempx2 = fieldx; tempx2 <= (fieldx + shipCount()); tempx2++)
	            {
	                if(tempx == tempx2)
	                    xokay = false;
	            }
	        }
	    }
            // Check for Y for ships > 1 square
	    else if( (shipiterator->shipxstart() == shipiterator->shipxstop() ) &&
                     (shipiterator->shipystart() != shipiterator->shipystop() ) )
	    {
	        for(tempy = shipiterator->shipystart(); tempy <= shipiterator->shipystop(); tempy++)
	        {
	            for(tempy2 = fieldy; tempy2 <= (fieldy + shipCount()); tempy2++)
	            {
	                if(tempy == tempy2)
	                    yokay = false;
	            }
	        }
	    }

	    kdDebug() << "*** Step 1 ***" << endl;
	    kdDebug() << "XOKAY: " << xokay << " YOKAY: " << yokay << endl;

	    if(button == LeftButton)
	    {
		int neighbourp, neighbourm, neighbourt, neighbourb;
	    	tempx = 0;
		if( fieldx > 0 )
		    neighbourm = getXYShipType(fieldx - 1, fieldy);
		else
		    neighbourm = 99;
		    neighbourp = getXYShipType(fieldx + shipCount(), fieldy);
                if(neighbourm == 99 && neighbourp == 99)
		{
		    for(tempx = fieldx; tempx <= (fieldx + shipCount()); tempx++)
		    {
		        if(fieldy > 0)
		            neighbourt = getXYShipType(tempx, fieldy - 1);
		        else
		            neighbourt = 99;
		        neighbourb = getXYShipType(tempx, fieldy + 1);
		        if( neighbourt != 99 || neighbourb != 99 )
		        {
    	                    xokay = false;
		            yokay = false;
	    kdDebug() << "*** Step 2 *r1**" << endl;
	    kdDebug() << "XOKAY: " << xokay << " YOKAY: " << yokay << endl;

		            break;
		        }
		    }
		    break;
		}
		else
		{
		    xokay = false;
		    yokay = false;
	    kdDebug() << "*** Step 2 *r2**" << endl;
	    kdDebug() << "XOKAY: " << xokay << " YOKAY: " << yokay << endl;

		    break;
		}
	    kdDebug() << "*** Step 2 *r3**" << endl;
	    kdDebug() << "XOKAY: " << xokay << " YOKAY: " << yokay << endl;

	    }
	    else if(button == RightButton)
	    {
		int neighbourp, neighbourm, neighbourt, neighbourb;
	    	tempy = 0;
		if(fieldy > 0)
		    neighbourm = getXYShipType(fieldx, fieldy - 1);
		else
		    neighbourm = 99;
	        neighbourp = getXYShipType(fieldx, fieldy + shipCount());
                if(neighbourm == 99 && neighbourp == 99)
		{
	            for(tempy = fieldy; tempy <= (fieldy + shipCount()); tempy++)
		    {
		        if(fieldx > 0)
			    neighbourt = getXYShipType(fieldx - 1, tempy);
			else
			    neighbourt = 99;
			neighbourb = getXYShipType(fieldx + 1, tempy);
			if(neighbourt != 99 || neighbourb != 99)
			{
			xokay = false;
		        yokay = false;
	    kdDebug() << "*** Step 2 *r1**" << endl;
	    kdDebug() << "XOKAY: " << xokay << " YOKAY: " << yokay << endl;

			    break;
			}
		    }
		    break;
		}
		else
		{
	    	    xokay = false;
		    yokay = false;
	    kdDebug() << "*** Step 2 *r2**" << endl;
	    kdDebug() << "XOKAY: " << xokay << " YOKAY: " << yokay << endl;

		    break;
	        }

	    }

	    kdDebug() << "*** Step 2 *r3**" << endl;
	    kdDebug() << "XOKAY: " << xokay << " YOKAY: " << yokay << endl;
	}

	if(xokay && !yokay)
	{
	    xokay = true;
	    yokay = true;
	}

	if(yokay && !xokay)
	{
	    xokay = true;
	    yokay = true;
	}

	kdDebug() << "*** Step 3 ***" << endl;
	kdDebug() << "XOKAY: " << xokay << " YOKAY: " << yokay << endl;
    }

    if(xokay && yokay)
    {
	shipsadded--;
	if(button == LeftButton)
	    shiplist.append(new KShip(fieldx, fieldx + shipCount(), fieldy, fieldy, shipCount()));
	else if(button == RightButton)
            shiplist.append(new KShip(fieldx, fieldx, fieldy, fieldy + shipCount(), shipCount()));

     	decideShipPlacing(button, fieldx, fieldy);
	if(shipsadded == 0)
	    emit lastShipAdded();
	return;
    }
    else if(xokay && !yokay)
    {
	shipsadded--;
    	if(button == LeftButton)
    	    shiplist.append(new KShip(fieldx, fieldx + shipCount(), fieldy, fieldy, shipCount()));
	else if(button == RightButton)
            shiplist.append(new KShip(fieldx, fieldx, fieldy, fieldy + shipCount(), shipCount()));

     	decideShipPlacing(button, fieldx, fieldy);
	if(shipsadded == 0)
	    emit lastShipAdded();
	return;
    }
    else if(!xokay && yokay)
    {
    	shipsadded--;
	if(button == LeftButton)
	    shiplist.append(new KShip(fieldx, fieldx + shipCount(), fieldy, fieldy, shipCount()));
	else if(button == RightButton)
    	    shiplist.append(new KShip(fieldx, fieldx, fieldy, fieldy + shipCount(), shipCount()));

        decideShipPlacing(button, fieldx, fieldy);
	if(shipsadded == 0)
	    emit lastShipAdded();
	return;
    }
    else if(!xokay && !yokay)
    {
        KMessageBox::information(0L, i18n( "You can't place the ship here." ));
	return;
    }
}

void KShipList::decideShipPlacing(int button, int fieldx, int fieldy)
{
    if(button == LeftButton)
    {
        if(fieldx + shipCount() <= 8)
            placeShipLMB(fieldx, fieldy);
    }
    else if(button == RightButton)
    {
	if(fieldy + shipCount() <= 8)
	    placeShipRMB(fieldx, fieldy);
    }
}

void KShipList::placeShipLMB(int fieldx, int fieldy)
{
    for(int i = 0; i < shipCount() + 1; i++)
        controlOwnFieldData(fieldx + i, fieldy, KBattleField::SHIP);
}

void KShipList::placeShipRMB(int fieldx, int fieldy)
{
    for(int i = 0; i < shipCount() + 1; i++)
        controlOwnFieldData(fieldx, fieldy + i, KBattleField::SHIP);
}

void KShipList::controlOwnFieldData(int fieldx, int fieldy, int type)
{
    emit ownFieldDataChanged(fieldx, fieldy, type);
}