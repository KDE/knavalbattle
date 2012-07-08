/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef PLAYERLABEL_H
#define PLAYERLABEL_H

#include <QLabel>
#include <QWidget>

class PlayerLabel : public QWidget 
{
private: 
    QLabel *m_name;
    QLabel *m_icon;
 
public:
    PlayerLabel(const QPixmap &icon, const QString &text, QWidget *parent);

    void setData(const QPixmap &icon, const QString &text);
    void setText(const QString &text);
};

#endif // PLAYERLABEL_H

