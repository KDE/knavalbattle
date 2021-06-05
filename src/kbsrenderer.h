/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef KBSRENDERER_H
#define KBSRENDERER_H

#include <KGameRenderer>

class Coord;

/**
  * Class to render KBattleShip graphical elements.
  */
class KBSRenderer : public KGameRenderer
{
public:
    /**
      * Create a new renderer instance. Each instance has a different cache.
      */
    KBSRenderer();

    ~KBSRenderer() override;

    /**
      * Set a new size for the elements.
      */
    void resize(int sz);
    void resize(const QSize& sz);

    /**
      * Return current pixmap size.
      */
    QSize size() const;
   
    Coord toLogical(const QPointF& p) const;
    QPointF toReal(const Coord& p) const;

private:
    QSize m_size;
};

#endif // KBSRENDERER_H
