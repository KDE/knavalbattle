/***************************************************************************
                              kbattleshipview.h
                             -------------------
    Developers: (c) 2000 Nikolas Zimmermann <nikoz@gymnasium-kerpen.de>
                (c) 2000 Daniel Molkentin <molkentin@kde.org>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KBATTLESHIPVIEW_H
#define KBATTLESHIPVIEW_H

#include <qpainter.h>
#include <qwidget.h>
#include "kbattleship.h"
#include "kbattlefieldtype.h"
#include "kbattlefield.h"

class KBattleshipView : public QWidget
{
    Q_OBJECT
    public:
        KBattleshipView( QWidget *parent = 0, const char *name = 0 );
        ~KBattleshipView();   
};

#endif
