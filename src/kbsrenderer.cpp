/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "kbsrenderer.h"
#include "coord.h"

#include <KgThemeProvider>
#include <KGameRenderedItem>

static KgThemeProvider *provider()
{
    KgThemeProvider *prov = new KgThemeProvider;
    prov->discoverThemes("appdata", QLatin1String("pictures"));
    return prov;
}

KBSRenderer::KBSRenderer()
: KGameRenderer(provider())
{
}

KBSRenderer::~KBSRenderer()
{
}

void KBSRenderer::resize(int sz)
{
    resize(QSize(sz, sz));
}

void KBSRenderer::resize(const QSize& sz)
{
    if (m_size != sz) {
        m_size = sz;
    }
}

QSize KBSRenderer::size() const
{
    return m_size;
}

Coord KBSRenderer::toLogical(const QPointF& p) const
{
    int x = static_cast<int>(p.x() / m_size.width());
    int y = static_cast<int>(p.y() / m_size.height());

    return Coord(x, y);
}

QPointF KBSRenderer::toReal(const Coord& c) const
{
    return QPointF(c.x * m_size.width(), c.y * m_size.height());
}
