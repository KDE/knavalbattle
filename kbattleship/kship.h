/***************************************************************************
                                      kship.h
                                  -----------------
    Developers: (c) 2000-2001 Nikolas Zimmermann <wildfox@kde.org>
                (c) 2000-2001 Daniel Molkentin <molkentin@kde.org>

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

#include <qobject.h>

class KShip : public QObject
{
    Q_OBJECT
    public:
        KShip(int, int, int, int, int);
        ~KShip();
    
        int shipxstart();
        int shipxstop();
        int shipystart();
        int shipystop();
        int shiptype();
    
    private:
        int m_shipxstart;
        int m_shipxstop;
        int m_shipystart;
        int m_shipystop;
        int m_shiptype;
};

#endif
