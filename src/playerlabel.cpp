/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#include <QHBoxLayout>

#include "playerlabel.h"

PlayerLabel::PlayerLabel(const QPixmap &icon, const QString &text, QWidget *parent)
: QWidget(parent)
{
    m_icon = new QLabel;
    m_icon->setPixmap(icon);

    m_name = new QLabel;
    m_name->setText(text);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_icon);
    layout->addSpacing(10);
    layout->addWidget(m_name);
    layout->addStretch();

    setLayout(layout);
}

void PlayerLabel::setData(const QPixmap &icon, const QString &text)
{
    m_icon->setPixmap(icon);
    m_name->setText("<font size=\"5\">" + text + "</font>");
}

void PlayerLabel::setText(const QString &text)
{
    m_name->setText("<font size=\"5\">" + text + "</font>");
}
