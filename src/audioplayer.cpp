/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "audioplayer.h"

#include <Phonon/MediaObject>
#include <KStandardDirs>

AudioPlayer::AudioPlayer(QObject* parent)
: QObject(parent)
, m_active(false)
{
    m_media = Phonon::createPlayer(Phonon::GameCategory);
    m_dir = KStandardDirs::locate("appdata", "sounds/");
}

void AudioPlayer::play(Sea::Player player, const HitInfo& info)
{
    if (m_active) {
        QString sound;
        if (info.type == HitInfo::HIT) {
            if (info.shipDestroyed) {
                sound = "ship-sink.ogg";
            }
            else {
                sound = player == Sea::PLAYER_A ? 
                                "ship-player1-shoot.ogg" : 
                                "ship-player2-shoot.ogg";
            }
        }
        else {
            sound = "ship-player-shoot-water.ogg";
        }
        
        if (!sound.isEmpty()) {
            kDebug() << "****** playing" << m_dir.filePath(sound);
            m_media->setCurrentSource(m_dir.filePath(sound));
            m_media->play();
        }
    }
}

