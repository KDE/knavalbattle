/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "audioplayer.h"

#include <phonon/MediaObject>

#include <QStandardPaths>

AudioPlayer::AudioPlayer(QObject* parent)
: QObject(parent)
, m_media(0)
{
    m_dir = QStandardPaths::locate(QStandardPaths::DataLocation, QLatin1Literal("sounds/"));
}

void AudioPlayer::play(Sea::Player player, const HitInfo& info)
{
    if (m_media) {
        QString sound;
        if (info.type == HitInfo::HIT) {
            if (info.shipDestroyed) {
                sound = QLatin1Literal("ship-sink.ogg");
            }
            else {
                sound = player == Sea::PLAYER_A ? 
                                QLatin1Literal("ship-player1-shoot.ogg") :
                                QLatin1Literal("ship-player2-shoot.ogg");
            }
        }
        else {
            sound = QLatin1Literal("ship-player-shoot-water.ogg");
        }
        
        if (!sound.isEmpty()) {
            qDebug() << "****** playing" << m_dir.filePath(sound);
            m_media->setCurrentSource(QUrl::fromLocalFile(m_dir.filePath(sound)));
            m_media->play();
        }
    }
}

void AudioPlayer::setActive(bool value) 
{ 
    if (value) {
        if (!m_media) {
            m_media = Phonon::createPlayer(Phonon::GameCategory);            
        }
    }
    else {
        delete m_media;
        m_media = 0;
    }
}



