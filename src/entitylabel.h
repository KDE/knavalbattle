/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef ENTITYLABEL_H
#define ENTITYLABEL_H

#include <kgamecanvas.h>

class EntityLabel : public QObject, public KGameCanvasGroup
{
Q_OBJECT
    KGameCanvasText* m_text;
    KGameCanvasPixmap* m_picture;
    KGameCanvasPixmap* m_pending_picture;
    KGameCanvasRectangle* m_background;
    
    QSize m_size;
public:
    EntityLabel(KGameCanvasAbstract* parent, const QFont& font, 
        const QString& text, const QSize& size);
    
    QPoint picturePos(KGameCanvasPixmap* picture);
    
    void setText(const QString& text);
    QString text() const;
    void showText();
    
    void update();
    void addPicture(KGameCanvasPixmap* picture);
    void resize(const QSize& size);
public slots:
    void setupPicture();
};

#endif // ENTITYLABEL_H

