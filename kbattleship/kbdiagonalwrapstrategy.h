/***************************************************************************
                           kbdiagonalwrapstrategy.h
                                 ----------
   Developers: (c) 2001 Kevin Krammer <kevin.krammer@gmx.at>

***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KBDIAGONALWRAPSTRATEGY_H
#define KBDIAGONALWRAPSTRATEGY_H

#include "kbstrategy.h"
#include "kbdestroyshipstrategy.h"
#include "kbdiagonalshotstrategy.h"

class KBDiagonalWrapStrategy : public KBStrategy
{
public:
    explicit KBDiagonalWrapStrategy(KBStrategy *parent = 0);
    virtual ~KBDiagonalWrapStrategy();

    virtual void init(KBattleField *field, const QRect &field_rect);
    virtual const QPoint nextShot();
    virtual bool hasMoreShots();
    virtual void shotAt(const QPoint &pos);

private:
    bool advance();
    bool advanceRightDown();
    bool advanceRightUp();
    bool advanceLeftDown();
    bool advanceLeftUp();

    int m_row;
    int m_column;

    QPoint m_start;
    KBDiagonalShotStrategy *m_child;
    KBDiagonalShotStrategy::Direction m_direction;

    KBDestroyShipStrategy *m_destroyer;
    bool m_destroying;
};

#endif
