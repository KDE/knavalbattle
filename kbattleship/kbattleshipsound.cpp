/***************************************************************************
                             kbattleshipsound.cpp
                             -------------------
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

#include <qfile.h>
#include "kbattleshipsound.h"
#include "kbattleshipsound.moc"

KBattleshipSound::KBattleshipSound() : QObject()
{
	playObject = Arts::PlayObject::null();
	playObjectFactory = Arts::PlayObjectFactory::null();
	soundserver = Arts::SimpleSoundServer::null();
}

KBattleshipSound::~KBattleshipSound()
{
}

void KBattleshipSound::turnOff()
{
	if(m_serverRunning && !m_soundError)
	{
		playObject = Arts::PlayObject::null();
		playObjectFactory = Arts::PlayObjectFactory::null();
		soundserver = Arts::SimpleSoundServer::null();
		m_serverRunning = false;
	}	
}

void KBattleshipSound::turnOn()
{
	if(!m_serverRunning && !m_soundError)
		initSoundServer();
}

bool KBattleshipSound::serverError()
{
	return m_soundError;
}

bool KBattleshipSound::initSoundServer()
{
	soundserver = Arts::Reference("global:Arts_SimpleSoundServer");
	playObjectFactory = Arts::Reference("global:Arts_PlayObjectFactory");
	if(soundserver.isNull())
	{
		KMessageBox::error(0L, i18n("Couldn't connect to aRts Soundserver. Sound deactivated"));
		playObjectFactory = Arts::PlayObjectFactory::null();
		soundserver = Arts::SimpleSoundServer::null();
		m_serverRunning = false;
		m_soundError = true;
	}
	else
	{
		QString soundDirCheck = locate("data", "kbattleship/sounds/");
		QString oneSoundCheck = locate("data", "kbattleship/sounds/ship-sink.mp3");
		if(!soundDirCheck.isEmpty() && !oneSoundCheck.isEmpty())
		{
			m_serverRunning = true;
			m_soundError = false;
		}
		else
		{
			KMessageBox::error(0L, i18n("You don't have KBattleship Sounds installed. Sound deactivated"));
			playObjectFactory = Arts::PlayObjectFactory::null();
			soundserver = Arts::SimpleSoundServer::null();
			m_serverRunning = false;
			m_soundError = true;
		}
	}

	return !m_soundError;
}

void KBattleshipSound::playSound(int file)
{
	if(m_serverRunning && !m_soundError)
	{
		QString soundDir = locate("data", "kbattleship/sounds/");
		QString playFile;

		switch(file)
		{
			case PLAYER1_SHOOT_HIT:
				playFile = soundDir + QString::fromLatin1("ship-player1-shoot.mp3");
				break;

			case PLAYER2_SHOOT_HIT:
				playFile = soundDir + QString::fromLatin1("ship-player2-shoot.mp3");
				break;

			case PLAYER_SHOOT_WATER:
				playFile = soundDir + QString::fromLatin1("ship-player-shoot-water.mp3");
				break;

			case SHIP_SINK:
				playFile = soundDir + QString::fromLatin1("ship-sink.mp3");
				break;
		}

		playObject = playObjectFactory.createPlayObject(playFile.latin1());
		if(!playObject.isNull())
			playObject.play();
	}
}
