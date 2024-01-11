/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kbsrenderer.h"
#include "coord.h"
// KDEGames
#include <KGameThemeProvider>
#include <KGameRenderedItem>

static KGameThemeProvider *provider()
{
    KGameThemeProvider *prov = new KGameThemeProvider;
    prov->discoverThemes(QStringLiteral("themes"));
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
