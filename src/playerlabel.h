/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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

