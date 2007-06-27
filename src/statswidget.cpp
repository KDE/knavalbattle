/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "statswidget.h"

#include "entity.h"

#include <QLabel>
#include <QLayout>

#include <KGameLCD>
#include <KIcon>
#include <KLocalizedString>

StatsWidget::StatsWidget(QWidget* parent)
: QWidget(parent)
, m_stats(0)
, m_editable(false)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;

    // title
    QHBoxLayout* tmp = new QHBoxLayout;
    m_player_icon = new QLabel(this);
//     m_player_icon->setPixmap(KIcon("user-female.png").pixmap(32, 32));
    tmp->addWidget(m_player_icon);
    tmp->addSpacing(5);
    m_player_name = new QLabel(this);
    m_player_name->setAlignment(Qt::AlignRight);
    m_player_name->setStyleSheet("font-size: 20px;");
    tmp->addWidget(m_player_name);
    
    mainLayout->addItem(tmp);
    
    mainLayout->addStretch();
    
    // shots
    tmp = new QHBoxLayout;
    QLabel* shots_label = new QLabel(i18n("Shots:"), this);
    tmp->addWidget(shots_label);
    tmp->addStretch();
    
    m_shots = new KGameLCD(3, this);
    tmp->addWidget(m_shots);
    
    mainLayout->addItem(tmp);
    
    // hits
    tmp = new QHBoxLayout;
    QLabel* hits_label = new QLabel(i18n("Hits:"), this);
    tmp->addWidget(hits_label);
    tmp->addStretch();
    
    m_hits = new KGameLCD(3, this);
    tmp->addWidget(m_hits);
    
    mainLayout->addItem(tmp);

    // misses
    tmp = new QHBoxLayout;
    QLabel* misses_label = new QLabel(i18n("Misses:"), this);
    tmp->addWidget(misses_label);
    tmp->addStretch();
    
    m_misses = new KGameLCD(3, this);
    tmp->addWidget(m_misses);
    
    mainLayout->addItem(tmp);
    
    setLayout(mainLayout);
}

void StatsWidget::setStats(Entity* entity)
{
    m_stats = entity->stats();
    Q_ASSERT(m_stats);
    connect(m_stats, SIGNAL(hitsChanged()), this, SLOT(updateHits()));
    connect(m_stats, SIGNAL(missesChanged()), this, SLOT(updateMisses()));
    connect(entity, SIGNAL(nick(int, QString)), this, SLOT(updateNick(int, QString)));
    
    m_player_name->setText(entity->nick());
    m_player_icon->setPixmap(entity->icon().pixmap(32, 32));
}

void StatsWidget::updateHits()
{
    Q_ASSERT(m_stats);
    m_shots->displayInt(m_stats->shots());
    m_hits->displayInt(m_stats->hits());
    m_hits->highlight();
}

void StatsWidget::updateMisses()
{
    Q_ASSERT(m_stats);
    m_shots->displayInt(m_stats->shots());
    m_misses->displayInt(m_stats->misses());
    m_misses->highlight();
}

void StatsWidget::updateNick(int, const QString& nick)
{
    m_player_name->setText(nick);
}

bool StatsWidget::editable() const
{
    return m_editable;
}

void StatsWidget::setEditable(bool value)
{
    m_editable = value;
}

QString StatsWidget::nick() const
{
    return m_player_name->text();
}

#include "statswidget.moc"


