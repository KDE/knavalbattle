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

#include "kbattleshipsound.moc"

KBattleshipSound::KBattleshipSound() : QObject()
{
}

KBattleshipSound::~KBattleshipSound()
{
}

void KBattleshipSound::turnOff()
{
    if(serverRunning && !soundError)
    {
	playObjectFactory = Arts::PlayObjectFactory::null();
	soundserver = Arts::SimpleSoundServer::null();
	serverRunning = false;
    }	
}

void KBattleshipSound::turnOn()
{
    if(!serverRunning && !soundError)
	initSoundServer();
}

bool KBattleshipSound::serverError()
{
    return soundError;
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
	serverRunning = false;
	soundError = true;
    }
    else
    {
	serverRunning = true;
	soundError = false;
    }
    
    return !soundError;
}

void KBattleshipSound::playSound(int file)
{
    if(serverRunning && !soundError)
    {
	QString soundDir = locate("data", "kbattleship/sounds/");
    	QString playFile;
    	
	switch(file)
	{
	    case PLAYER1_SHOOT_HIT:
		playFile = soundDir + QString("ship-player1-shoot.mp3");
		break;
		
	    case PLAYER2_SHOOT_HIT:
	        playFile = soundDir + QString("ship-player2-shoot.mp3");
	        break;

	    case PLAYER_SHOOT_WATER:
	        playFile = soundDir + QString("ship-player-shoot-water.mp3");
	        break;
    
	    case SHIP_SINK:
	        playFile = soundDir + QString("ship-sink.mp3");
	        break;
	}
	
	playObject = playObjectFactory.createPlayObject(playFile.latin1());
	playObject.play();
    }
}
