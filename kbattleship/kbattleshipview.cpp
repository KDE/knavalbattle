/***************************************************************************
                             kbattleshipview.cpp
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

// include files for Qt
#include <qprinter.h>
#include <qpainter.h>

// application specific includes
#include "kbattleshipview.h"
#include "kbattleship.h"

#include "kbattleshipview.moc"

KBattleshipView::KBattleshipView(QWidget *parent, const char *name) : QWidget(parent, name)
{
  setBackgroundMode(PaletteBase);
}

KBattleshipView::~KBattleshipView()
{
}

