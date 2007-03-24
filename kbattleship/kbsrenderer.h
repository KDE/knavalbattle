#ifndef KBSRENDERER_H
#define KBSRENDERER_H

#include <QString>
#include <QPixmap>
#include <QHash>
#include "ship.h"

class KSvgRenderer;

/**
  * Class to render KBattleShip graphical elements.
  */
class KBSRenderer
{
    typedef QHash<QString, QPixmap> Cache; // use QCache maybe?
public:
    /**
      * Create a new renderer instance. Each instance has a different cache.
      */
    KBSRenderer(const QString& path);

    ~KBSRenderer();

    /**
      * Set a new size for the elements. Invalidate cache.
      */
    void resize(int sz);
    void resize(const QSize& sz);

    /**
      * Return current pixmap size.
      */
    QSize size() const;

    /**
      * Render an item ensuring it is in the cache.
      */
    QPixmap render(const QString& id, int xScale = 1, int yScale = 1);
    
    Coord toLogical(const QPoint& p) const;
    QPoint toReal(const Coord& p) const;
protected:
    QPixmap render(const QString& id, const QSize& sz);
private:
    KSvgRenderer* m_renderer;
    QSize m_size;

    Cache m_cache;
};

#endif // KBSRENDERER_H
