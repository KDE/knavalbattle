/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <Phonon/MediaObject>
#include "sea.h"

class AudioPlayer : public QObject
{
    Phonon::MediaObject *m_player;
    QString m_dir;
    bool m_active;
public:
    AudioPlayer(QObject* parent);

    void play(Sea::Player player, const HitInfo& info);
    void activate(bool value) { m_active = value; }
};

#endif // AUDIOPLAYER_H

