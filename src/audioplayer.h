/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include "sea.h"

class KGameSound;

class AudioPlayer : public QObject
{
Q_OBJECT

public:
    explicit AudioPlayer(QObject* parent);

    void play(Sea::Player player, const HitInfo& info);
    void setActive(bool value);

private:
    KGameSound *m_sink;
    KGameSound *m_shootA;
    KGameSound *m_shootB;
    KGameSound *m_shootWater;
};

#endif // AUDIOPLAYER_H

