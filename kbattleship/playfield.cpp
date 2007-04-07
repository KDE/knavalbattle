/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include <QResizeEvent>
#include <QLayout>

#include "playfield.h"
#include "seaview.h"
#include "controller.h"

PlayField::PlayField(QWidget* parent)
: QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout;

    m_sea = new SeaView(this);
    m_controller = new OnePlayerController(this, m_sea, Sea::Player(0));
    m_sea->setController(m_controller);
    
    layout->addWidget(m_sea);
    
    setLayout(layout);
}


void PlayField::newGame()
{
    delete m_controller;
    m_controller = new OnePlayerController(this, m_sea, Sea::Player(0));
    m_sea->setController(m_controller);
}

#include "playfield.moc"


