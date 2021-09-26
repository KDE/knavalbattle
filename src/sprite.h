/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SPRITE_H
#define SPRITE_H

#include <KGameRenderedItem>

#include "coord.h"

class KBSRenderer;

class Sprite : public KGameRenderedItem
{
    Coord m_scale;
    QString m_name;
    bool m_rotated;
public:
    Sprite(KBSRenderer* renderer, const Coord& scale, const QString& name, bool rotated);
    ~Sprite() override;

    void refresh(KBSRenderer* renderer);
    void turnRed();
    void turnGreen();
};

#endif // SPRITE_H

