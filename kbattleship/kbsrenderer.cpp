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

QPixmap KBSRenderer::render(const PixmapData& data, const QSize& sz)
{
    if (!m_cache.contains(data)) {
        kDebug() << "caching " << data.name << endl;
        if (!m_renderer->elementExists(data.name)) {
            kDebug() << "no element " << data.name << "\n";
            return QPixmap();
        }
        QImage tmp(sz, QImage::Format_ARGB32_Premultiplied);
        tmp.fill(Qt::blue);
        {
            QSize renderSize;
            
            QPainter p(&tmp);
            if (data.rotated) {
                kDebug() << "rotating " << data.name << endl;
//                 p.setWorldMatrix(QMatrix(0, -1, 1, 0, sz.width(), 0));
                p.translate(QPoint(sz.width(), 0));
                p.rotate(90);
                renderSize = QSize(sz.height(), sz.width());
            }
            else {
                renderSize = sz;
            }
            
            m_renderer->render(&p, data.name, QRectF(QPointF(0, 0), renderSize));
        }
        m_cache[data] = QPixmap::fromImage(tmp);
    }
    else {
        kDebug() << "using chached element " << data.name << endl;
    }

    return m_cache.value(data);
}

QPixmap KBSRenderer::render(const QString& id, bool rotated, int xScale, int yScale)
{
    return render(PixmapData(id, rotated), 
        QSize(m_size.width() * xScale, m_size.height() * yScale));
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

KBSRenderer::PixmapData::PixmapData(const QString& name, bool rotated)
: name(name)
, rotated(rotated)
{
}

bool KBSRenderer::PixmapData::operator==(const PixmapData& other) const
{
    return other.name == name &&
            other.rotated == rotated;
}

uint qHash(const KBSRenderer::PixmapData& data) {
    QString s = data.name;
    if (data.rotated) {
        s += "__rotated";
    }
    return qHash(s);
}


