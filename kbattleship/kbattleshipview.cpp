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
    QPainter ownpainter( this );
    QPainter enemypainter( this );
    setFixedSize( 600, 400 );
    setBackgroundMode( PaletteBase );
    KBattleFieldType owntype;
    KBattleFieldType enemytype;
    owntype.setType( KBattleFieldType::OWNFIELD );
    enemytype.setType( KBattleFieldType::ENEMYFIELD );
    ownfield = new KBattleField( this, owntype, &ownpainter );
    enemyfield = new KBattleField( this, enemytype, &enemypainter );
}

KBattleshipView::~KBattleshipView()
{
}

void KBattleshipView::paintEvent( QPaintEvent * )
{
    kdDebug() << "Repaint!" << endl;
    QPainter ownpainter( this );
    QPainter enemypainter( this );
    ownfield->drawField( &ownpainter );
    enemyfield->drawField( &enemypainter );
}

