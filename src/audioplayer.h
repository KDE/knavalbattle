/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include "sea.h"

class KgSound;

class AudioPlayer : public QObject
{
Q_OBJECT

public:
    AudioPlayer(QObject* parent);

    void play(Sea::Player player, const HitInfo& info);
    void setActive(bool value);

private:
    KgSound *m_sink;
    KgSound *m_shootA;
    KgSound *m_shootB;
    KgSound *m_shootWater;
};

#endif // AUDIOPLAYER_H

