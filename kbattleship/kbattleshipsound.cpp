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
	QString picDir;
	QStringList picDirl = stdDirs->findDirs( "data", "kbattleship" );
	for( QStringList::Iterator it = picDirl.begin(); it != picDirl.end(); it++ )
	{
	    picDir = *it;
	    break;
	}

	picDir = picDir + "sounds/";

    	switch( file )
	{
	    case SHIP_EXPLODE:
		playObject = playObjectFactory.createPlayObject( QString( picDir + QString( "ship-explode.mp3" ) ).latin1() );
		break;
		
	    case PLAYER1_SHOOT_HIT:
		playObject = playObjectFactory.createPlayObject( QString( picDir + QString( "ship-player1-shoot.mp3" ) ).latin1() );
		break;
		
	    case PLAYER2_SHOOT_HIT:
		playObject = playObjectFactory.createPlayObject( QString( picDir + QString( "ship-player2-shoot.mp3" ) ).latin1() );
		break;

	    case PLAYER_SHOOT_WATER:
		playObject = playObjectFactory.createPlayObject( QString( picDir + QString( "ship-player-shoot-water.mp3" ) ).latin1() );
		break;
    
	    case SHIP_SINK:
		playObject = playObjectFactory.createPlayObject( QString( picDir + QString( "ship-sink.mp3" ) ).latin1() );
		break;
	}
	playObject.play();
    }
}

bool KBattleshipSound::isRunning()
{
    return serverRunning;
}
