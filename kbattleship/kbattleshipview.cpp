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

#include "kbattleshipview.moc"

KBattleshipView::KBattleshipView( QWidget *parent, const char *name ) : QWidget( parent, name )
{
    QPainter painter( this );
    setFixedSize( 500, 300 );
    setBackgroundMode( PaletteBase );
    KBattleFieldType type;
    type.setType( KBattleFieldType::OWNFIELD );
    ownfield = new KBattleField( this, type, &painter );
}

KBattleshipView::~KBattleshipView()
{
}

void KBattleshipView::paintEvent( QPaintEvent * )
{
    kdDebug() << "Repaint!" << endl;
    QPainter painter( this );
    ownfield->drawField( &painter );
}

