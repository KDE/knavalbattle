/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "audioplayer.h"

#include <KGameSound>

#include <QDir>
#include <QStandardPaths>

AudioPlayer::AudioPlayer(QObject* parent)
: QObject(parent)
, m_sink(nullptr)
, m_shootA(nullptr)
, m_shootB(nullptr)
, m_shootWater(nullptr)
{

}

void AudioPlayer::play(Sea::Player player, const HitInfo& info)
{
    KGameSound *sound;
    if (info.type == HitInfo::HIT) {
        if (info.shipDestroyed) {
            sound = m_sink;
        }
        else {
            sound = player == Sea::PLAYER_A ? m_shootA : m_shootB;
        }
    }
    else {
        sound = m_shootWater;
    }

    if (sound) {
        sound->start();
    }
}

void AudioPlayer::setActive(bool value)
{
    if (value) {
        if (!m_sink) {
            const QDir dir = QStandardPaths::locate(QStandardPaths::AppDataLocation, QStringLiteral("sounds/"), QStandardPaths::LocateDirectory);
            m_sink = new KGameSound(dir.filePath(QStringLiteral("ship-sink.ogg")), this);
            m_shootA = new KGameSound(dir.filePath(QStringLiteral("ship-player1-shoot.ogg")), this);
            m_shootB = new KGameSound(dir.filePath(QStringLiteral("ship-player2-shoot.ogg")), this);
            m_shootWater = new KGameSound(dir.filePath(QStringLiteral("ship-player-shoot-water.ogg")), this);
        }
    }
    else {
        delete m_sink;
        delete m_shootA;
        delete m_shootB;
        delete m_shootWater;
        m_sink = nullptr;
        m_shootA = nullptr;
        m_shootB = nullptr;
        m_shootWater = nullptr;
    }
}

#include "moc_audioplayer.cpp"
