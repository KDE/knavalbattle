/*
  Copyright (c) 2000-2001 Nikolas Zimmermann <wildfox@kde.org>
            (c) 2000-2001 Daniel Molkentin <molkentin@kde.org>
            (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include <QResizeEvent>
#include <QLayout>
#include <kscoredialog.h>
#include <klocale.h>

#include "playfield.h"
#include "seaview.h"
#include "controller.h"

PlayField::PlayField(QWidget* parent)
: QWidget(parent)
, m_human_player(-1)
{
    QVBoxLayout* layout = new QVBoxLayout;

    m_sea = new SeaView(this);
    m_controller = 0;
    setup(new OnePlayerController(this, m_sea, Sea::Player(0)));
    m_human_player = 0;
    
    layout->addWidget(m_sea);
    
    setLayout(layout);

    m_highscores = new KScoreDialog(
        KScoreDialog::Name | KScoreDialog::Score | 
        KScoreDialog::Custom1 | 
        KScoreDialog::Custom2 | 
        KScoreDialog::Custom3, 
        this);
    m_highscores->addField(KScoreDialog::Custom1, i18n("Shots"), "shots");
    m_highscores->addField(KScoreDialog::Custom2, i18n("Hits"), "hits");
    m_highscores->addField(KScoreDialog::Custom3, i18n("Water"), "water");
}

void PlayField::setup(Controller* controller)
{
    delete m_controller;
    m_controller = controller;
    m_sea->setController(m_controller);
    kDebug() << "connecting" << endl;
    connect(m_controller, SIGNAL(gameOver(Sea::Player)), 
            this, SLOT(gameOver(Sea::Player)));
}

void PlayField::newGame()
{
    setup(new OnePlayerController(this, m_sea, Sea::Player(0)));
    m_human_player = 0;
}

void PlayField::newSimulation()
{
    setup(new TwoMachinesController(this, m_sea));
    m_human_player = -1;
}

void PlayField::highscores()
{
    m_highscores->exec();
}

void PlayField::gameOver(Sea::Player winner)
{
    kDebug() << "playfield game over: " << m_human_player << endl;
    if (m_human_player == winner) {
        const Stats* stats = m_controller->stats();
        kDebug() << "stats = " << stats << endl;
        if (stats) {
            KScoreDialog::FieldInfo info;
//             info[KScoreDialog::Name] = "my nick";
            info[KScoreDialog::Custom1] = QString::number(stats->shots());
            info[KScoreDialog::Custom2] = QString::number(stats->hits());
            info[KScoreDialog::Custom3] = QString::number(stats->misses());
        
            kDebug() << "score = " << stats->score() << endl;
            if (m_highscores->addScore(stats->score(), info, true, false)) {
                highscores();
            }
        }
    }
}

#include "playfield.moc"


