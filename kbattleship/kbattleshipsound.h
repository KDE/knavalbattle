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
#include <qstring.h>
#include <qobject.h>
#include <arts/dispatcher.h>
#include <arts/soundserver.h>

using namespace std;
using namespace Arts;

class KBattleshipSound : public QObject
{
    Q_OBJECT
    public:
        KBattleshipSound();
        ~KBattleshipSound();	
	
    public slots:
        void initSoundServer();
	void playSound( QString file );
	
    private:
	SimpleSoundServer *soundserver;
	bool soundRunning;
	bool isRunning();
};

#endif

