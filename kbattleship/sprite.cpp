#include <kdebug.h>

#include "sprite.h"
#include "kbsrenderer.h"

Sprite::Sprite(KGameCanvasAbstract* parent, KBSRenderer* renderer, 
            const Coord& scale, const QString& name, bool rotated)
: KGameCanvasPixmap(parent)
, m_scale(scale)
, m_name(name)
, m_rotated(rotated)
{
    update(renderer);
}

Sprite::~Sprite()
{
    kDebug() << "sprite " << m_name << " dying" << endl;
}

void Sprite::update(KBSRenderer* renderer)
{
    setPixmap(renderer->render(m_name, m_rotated, m_scale.x, m_scale.y));
}
