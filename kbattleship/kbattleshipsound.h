/***************************************************************************
                                  kbattleshipsound.h
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

#ifndef KBATTLESHIPSOUND_H
#define KBATTLESHIPSOUND_H

#include <arts/soundserver.h>

#include <kglobal.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>

#include <qstring.h>

class KBattleshipSound
{
public:
	enum{SHIP_SINK, PLAYER1_SHOOT_HIT, PLAYER2_SHOOT_HIT, PLAYER_SHOOT_WATER};
	KBattleshipSound();

	bool initSoundServer();
	bool serverError();

	void turnOn();
	void turnOff();

	void playSound(int file);

private:
	Arts::SimpleSoundServer soundserver;
	Arts::PlayObjectFactory playObjectFactory;
	Arts::PlayObject playObject;

	bool m_serverRunning;
	bool m_soundError;
};

#endif
