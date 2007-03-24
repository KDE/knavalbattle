#include "sprite.h"
#include "kbsrenderer.h"

Sprite::Sprite(KGameCanvasAbstract* parent, KBSRenderer* renderer, 
            const Coord& scale, const QString& name)
: KGameCanvasPixmap(parent)
, m_scale(scale)
, m_name(name)
{
    update(renderer);
}

void Sprite::update(KBSRenderer* renderer)
{
    setPixmap(renderer->render(m_name, m_scale.x, m_scale.y));
}

