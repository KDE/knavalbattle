/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "playerlabel.h"

#include <QFont>
#include <QPixmap>

PlayerLabel::PlayerLabel(const QPixmap& icon, const QString& text,
    KGameCanvasAbstract* parent)
: KGameCanvasGroup(parent)
{
    m_icon = new KGameCanvasPixmap(icon, this);
    m_icon->show();
    
    QFont font;
    font.setPixelSize(20);
    m_name = new KGameCanvasText(text, Qt::black, font, 
        KGameCanvasText::HLeft, KGameCanvasText::VTop, this);
    m_name->show();
}

void PlayerLabel::setData(const QPixmap& icon, const QString& text)
{
    m_icon->setPixmap(icon);
    m_name->setText(text);
    
    update();
}

void PlayerLabel::setText(const QString& text)
{
    m_name->setText(text);
    update();
}

void PlayerLabel::update()
{
    m_icon->moveTo(0, 0);
    m_name->moveTo(m_icon->pixmap().width() + MARGIN, 0);
}

int PlayerLabel::height() const
{
    return m_icon->pixmap().height();
}

