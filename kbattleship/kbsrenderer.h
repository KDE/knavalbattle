/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

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
    struct PixmapData {
        QString name;
        bool rotated;
        bool operator==(const PixmapData& other) const;
        
        PixmapData(const QString& name, bool rotated);
    };
    friend uint qHash(const PixmapData&);
    typedef QHash<PixmapData, QPixmap> Cache; // use QCache maybe?
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
    QPixmap render(const QString& id, bool rotated = false, int xScale = 1, int yScale = 1);
    
    Coord toLogical(const QPoint& p) const;
    QPoint toReal(const Coord& p) const;
protected:
    QPixmap render(const PixmapData& data, const QSize& sz);
private:
    KSvgRenderer* m_renderer;
    QSize m_size;

    Cache m_cache;
};

#endif // KBSRENDERER_H
