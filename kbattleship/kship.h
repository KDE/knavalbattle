/***************************************************************************
                                       kship.h
                                  -----------------
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
 
#ifndef KSHIP_H
#define KSHIP_H

#include <kdebug.h>
#include <qobject.h>

class KShip : public QObject
{
    Q_OBJECT
    public:
        KShip( int shipxstart, int shipxstop, int shipystart, int shipxstart, int shiplength );
        ~KShip();
    
		int shipxstart();
		int shipxstop();
		int shipystart();
		int shipystop();
		int shiptype();
	
    private:
		int internalshipxstart;
		int internalshipxstop;
		int internalshipystart;
		int internalshipystop;
		int internalshiptype;
};

#endif

