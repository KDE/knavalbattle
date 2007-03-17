#include "kbsrenderer.h"
#include "kbattlefield.h"
#include <ksvgrenderer.h>
#include <kdebug.h>
#include <QPainter>

KBSRenderer::KBSRenderer(const QString& path)
        : m_renderer(new KSvgRenderer(path, 0))
{
    m_typenames[KBattleField::WATER] = "water";
    m_typenames[KBattleField::HIT] = "hit";
    m_typenames[KBattleField::DEATH] = "death";
    m_typenames[KBattleField::BORDER] = "border";
    m_typenames[KBattleField::SHIP1P1] = "ship1-1";
    m_typenames[KBattleField::SHIP2P1] = "ship2-1";
    m_typenames[KBattleField::SHIP2P2] = "ship2-2";
    m_typenames[KBattleField::SHIP3P1] = "ship3-1";
    m_typenames[KBattleField::SHIP3P2] = "ship3-2";
    m_typenames[KBattleField::SHIP3P3] = "ship3-3";
    m_typenames[KBattleField::SHIP4P1] = "ship4-1";
    m_typenames[KBattleField::SHIP4P2] = "ship4-2";
    m_typenames[KBattleField::SHIP4P3] = "ship4-3";
    m_typenames[KBattleField::SHIP4P4] = "ship4-4";
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
    m_size = sz;
}

QSize KBSRenderer::size() const
{
    return m_size;
}

void KBSRenderer::resizeBackground(const QSize& sz)
{
    m_background_size = sz;
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

QPixmap KBSRenderer::render(const QString& id)
{
    return render(id, m_size);
}

QPixmap KBSRenderer::render(int type, bool rotate)
{
    if (!m_typenames.contains(type))
        return QPixmap();
    else {
        QString id = m_typenames.value(type);
        if (!rotate && id.startsWith("ship"))
            id += "-r";
        return render(id);
    }
}

QPixmap KBSRenderer::renderBackground()
{
    return render("background", m_background_size);
}

