/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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
