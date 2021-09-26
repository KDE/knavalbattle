/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sprite.h"

#include <QGraphicsEffect>

#include "kbsrenderer.h"

Sprite::Sprite(KBSRenderer* renderer, const Coord& scale, const QString& name, bool rotated)
: KGameRenderedItem(renderer, name)
, m_scale(scale)
, m_name(name)
, m_rotated(rotated)
{
    refresh(renderer);
}

Sprite::~Sprite()
{
}

void Sprite::turnGreen()
{
    QGraphicsColorizeEffect * fx = new QGraphicsColorizeEffect();
    fx->setColor(QColor(Qt::green));
    setGraphicsEffect(fx);
}

void Sprite::turnRed()
{
    QGraphicsColorizeEffect * fx = new QGraphicsColorizeEffect();
    fx->setColor(QColor(Qt::red));
    setGraphicsEffect(fx);
}

void Sprite::refresh(KBSRenderer* renderer)
{
    if (m_rotated)
    {
        setTransformOriginPoint(renderer->size().width() / 2,
                                renderer->size().height() / 2);
        setRotation(90);
       
        QSize renderSize(renderer->size().height() * m_scale.y,
                         renderer->size().width()  * m_scale.x);
        setRenderSize(renderSize);
    }
    else
    {
        QSize renderSize(renderer->size().width()  * m_scale.x,
                         renderer->size().height() * m_scale.y);
        setRenderSize(renderSize);   
    }
}
