/***************************************************************************
                             kbattleshipsound.cpp
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

#include "kbattleshipsound.moc"

using namespace Arts;

KBattleshipSound::KBattleshipSound()
{
    initSoundServer();
}

KBattleshipSound::~KBattleshipSound()
{
}

void KBattleshipSound::initSoundServer()
{
    soundserver = Arts::Reference( "global:Arts_SimpleSoundServer" );
    playObjectFactory = Arts::Reference( "global:Arts_PlayObjectFactory" );
    if( soundserver.isNull() )
    {
	KMessageBox::error( 0L, i18n( "Couldn't connect to Arts Soundserver. Sound deactivated" ) );
	serverRunning = false;
    }
    else
	serverRunning = true;
}

void KBattleshipSound::playSound( int file )
{
    if( isRunning() )
    {
	KStandardDirs *stdDirs = KGlobal::dirs();
	QString soundDir;
	QStringList soundDirl = stdDirs->findDirs( "data", "kbattleship" );
	for( QStringList::Iterator it = soundDirl.begin(); it != soundDirl.end(); it++ )
	{
	    soundDir = *it;
	    break;
	}

	soundDir = soundDir + "sounds/";

	QString playFile;
    	switch( file )
	{
	    case PLAYER1_SHOOT_HIT:
		playFile = soundDir + QString( "ship-player1-shoot.mp3" );
		break;
		
	    case PLAYER2_SHOOT_HIT:
		playFile = soundDir + QString( "ship-player2-shoot.mp3" );
		break;

	    case PLAYER_SHOOT_WATER:
		playFile = soundDir + QString( "ship-player-shoot-water.mp3" );
		break;
    
	    case SHIP_SINK:
		playFile = soundDir + QString( "ship-sink.mp3" );
		break;
	}
	
	playObject = playObjectFactory.createPlayObject( playFile.latin1() );
	playObject.play();
    }
}

bool KBattleshipSound::isRunning()
{
    return serverRunning;
}
