/***************************************************************************
                                  kbattleshipsound.h
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
 
#ifndef KBATTLESHIPSOUND_H
#define KBATTLESHIPSOUND_H

#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstddirs.h>
#include <kglobal.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qobject.h>
#include <arts/soundserver.h>

class KBattleshipSound : public QObject
{
    Q_OBJECT
    public:
	enum{ SHIP_SINK, PLAYER1_SHOOT_HIT, PLAYER2_SHOOT_HIT, PLAYER_SHOOT_WATER };
        KBattleshipSound();
        ~KBattleshipSound();	

	void playSound( int file );
        void initSoundServer();
	
    private:
	Arts::SimpleSoundServer soundserver;
	Arts::PlayObjectFactory playObjectFactory;
	Arts::PlayObject playObject;
	bool isRunning();
        bool serverRunning;
};

#endif

