/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "entitylabel.h"
#include "animator.h"
#include "animation.h"

#include <kdebug.h>

EntityLabel::EntityLabel(KGameCanvasAbstract* parent, const QFont& font, const QString& text, const QSize& size)
: KGameCanvasGroup(parent)
, m_picture(0)
, m_pending_picture(0)
, m_size(size)
{
    m_background = new KGameCanvasRectangle(QColor(0x20, 0x4a, 0x87), m_size, this);
    m_background->show();

    m_text = new KGameCanvasText(text, QColor(255,255,255), font, 
        KGameCanvasText::HLeft, KGameCanvasText::VCenter, this);
    m_text->show();
        
    update();
}

QPoint EntityLabel::picturePos(KGameCanvasPixmap* picture)
{
    int h = m_size.height();
    return QPoint(
        h - picture->pixmap().width(),
        h - picture->pixmap().height()) / 2;
}

void EntityLabel::update()
{
    if (m_picture) {
        m_picture->moveTo(picturePos(m_picture));
    }
    m_text->moveTo(m_size.height(), m_size.height() / 2);
    
//     kDebug() << "resize background " << m_size << endl;
    m_background->setSize(m_size);
    m_background->moveTo(0, 0);
}

void EntityLabel::resize(const QSize& size)
{
    m_size = size;
    update();
}

void EntityLabel::addPicture(KGameCanvasPixmap* picture)
{
    m_pending_picture = picture;
}


void EntityLabel::setText(const QString& text)
{
    m_text->setText(text);
    update();
}

QString EntityLabel::text() const
{
    return m_text->text();
}

void EntityLabel::setupPicture()
{
    Q_ASSERT(m_pending_picture);
    m_picture = m_pending_picture;
    m_pending_picture = 0;
    m_picture->putInCanvas(this);
    update();
}

void EntityLabel::showText()
{
    Animation* a = new FadeAnimation(m_text, 0, 255, 500);
    Animator::instance()->add(a);
}

#include "entitylabel.moc"

