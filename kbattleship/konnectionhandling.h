/***************************************************************************
                                 konnectionhandling.h
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
 
#ifndef KONNECTIONHANDLING_H
#define KONNECTIONHANDLING_H

#include <kdebug.h>
#include <qobject.h>
#include "kbattleshipserver.h"
#include "kbattleshipclient.h"

class KonnectionHandling : public QObject
{
    Q_OBJECT
    public:
        KonnectionHandling( KBattleshipServer *server );
        KonnectionHandling( KBattleshipClient *client );
        ~KonnectionHandling();
	
    public slots:
        void serverGotNewClient();
	void serverWroteToClient();
	void serverLostClient();	
	void clientLostServer();
	void clientSocketError( int error );
};

#endif

