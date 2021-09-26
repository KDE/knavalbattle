/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "playerlabel.h"

#include <QHBoxLayout>

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
    m_name->setText(QLatin1String("<font size=\"5\">") + text + QLatin1String("</font>"));
}

void PlayerLabel::setText(const QString &text)
{
    m_name->setText(QLatin1String("<font size=\"5\">") + text + QLatin1String("</font>"));
}
