/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <phonon/audioplayer.h>
#include "sea.h"

class AudioPlayer : private Phonon::AudioPlayer
{
    QString m_dir;
    KUrl path(const QString& soundName) const;
public:
    AudioPlayer(QObject* parent);

    void play(Sea::Player player, const HitInfo& info);
};

#endif // AUDIOPLAYER_H

