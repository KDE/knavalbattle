#include "kbsrenderer.h"
#include <ksvgrenderer.h>
#include <kdebug.h>
#include <QPainter>

KBSRenderer::KBSRenderer(const QString& path)
{
    m_renderer = new KSvgRenderer(path, 0);
}

KBSRenderer::~KBSRenderer()
{
    delete m_renderer;
}

void KBSRenderer::resize(int sz)
{
    resize(QSize(sz, sz));
}

void KBSRenderer::resize(const QSize& sz)
{
    if (m_size != sz) {
        m_size = sz;
        m_cache.clear();
    }
}

QSize KBSRenderer::size() const
{
    return m_size;
}

QPixmap KBSRenderer::render(const QString& id, const QSize& sz)
{
    if (!m_cache.contains(id)) {
        if (!m_renderer->elementExists(id)) {
            kDebug() << "no element " << id << "\n";
            return QPixmap();
        }
        QImage tmp(sz, QImage::Format_ARGB32_Premultiplied);
        tmp.fill(0);
        {
            QPainter p(&tmp);
            //      p.setViewport(QRect(0, 0, sz.width(), sz.height()));
            //p.fillRect(QRect(0, 0, sz.width(), sz.height()), Qt::blue);
            m_renderer->render(&p, id, QRectF(QPointF(0, 0), sz));
        }
        m_cache[id] = QPixmap::fromImage(tmp);
    }

    return m_cache.value(id);
}

QPixmap KBSRenderer::render(const QString& id, int xScale, int yScale)
{
    return render(id, QSize(m_size.width() * xScale, m_size.height() * yScale));
}

Coord KBSRenderer::toLogical(const QPoint& p) const
{
    int x = p.x();
    x = x >= 0 ? x / m_size.width() : x / m_size.width() - 1;
    int y = p.y();
    y = y >= 0 ? y / m_size.height() : y / m_size.height() - 1;
    return Coord(x, y);
}

QPoint KBSRenderer::toReal(const Coord& c) const
{
    return QPoint(c.x * m_size.width(), c.y * m_size.height());
}

