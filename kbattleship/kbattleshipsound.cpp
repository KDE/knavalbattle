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

KBattleshipSound::KBattleshipSound() : QObject()
{
    initSoundServer();
}

KBattleshipSound::~KBattleshipSound()
{
}

void KBattleshipSound::initSoundServer()
{
    soundserver = new SimpleSoundServer;
    *soundserver = Arts::Reference( "global:Arts_SimpleSoundServer" );
    if( soundserver->isNull() )
    {
	KMessageBox::error( 0L, i18n( "Couldn't connect to Arts Soundserver. Sound deactivated" ) );
	soundRunning = false;
    }
    else
    {
	soundRunning = true;
    }
	
}

void KBattleshipSound::playSound( QString file )
{
    if( isRunning() )
	soundserver->play( file.latin1() );
}

bool KBattleshipSound::isRunning()
{
    return soundRunning;
}
