/***************************************************************************
                             khighscoredialog.cpp
                             -------------------
    Developers: (c) 2000-2001 Nikolas Zimmermann <wildfox@kde.org>
                (c) 2000-2001 Daniel Molkentin <molkentin@kde.org>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qlcdnumber.h>
#include <qlabel.h>
#include <kdebug.h>
#include <qpushbutton.h>
#include "khighscoredialog.moc"

KHighscoreDialog::KHighscoreDialog(QWidget *parent, const char *name) : highscoreDlg(parent, name)
{
    setMinimumSize(150, 300);
    m_score = new KHighscore(this);
}

KHighscoreDialog::~KHighscoreDialog()
{
}

void KHighscoreDialog::load()
{
    Name1->setText(m_score->readEntry(0, "Name", "-"));
    Name2->setText(m_score->readEntry(1, "Name", "-"));
    Name3->setText(m_score->readEntry(2, "Name", "-"));
    Name4->setText(m_score->readEntry(3, "Name", "-"));
    Name5->setText(m_score->readEntry(4, "Name", "-"));
    Name6->setText(m_score->readEntry(5, "Name", "-"));
    Name7->setText(m_score->readEntry(6, "Name", "-"));
    Name8->setText(m_score->readEntry(7, "Name", "-"));
    Name9->setText(m_score->readEntry(8, "Name", "-"));
    Name10->setText(m_score->readEntry(9, "Name", "-"));

    ScoreLCD1->display(m_score->readNumEntry(0, "Score", 0));
    ScoreLCD2->display(m_score->readNumEntry(1, "Score", 0));
    ScoreLCD3->display(m_score->readNumEntry(2, "Score", 0));
    ScoreLCD4->display(m_score->readNumEntry(3, "Score", 0));
    ScoreLCD5->display(m_score->readNumEntry(4, "Score", 0));
    ScoreLCD6->display(m_score->readNumEntry(5, "Score", 0));
    ScoreLCD7->display(m_score->readNumEntry(6, "Score", 0));
    ScoreLCD8->display(m_score->readNumEntry(7, "Score", 0));
    ScoreLCD9->display(m_score->readNumEntry(8, "Score", 0));
    ScoreLCD10->display(m_score->readNumEntry(9, "Score", 0));

    ShotLCD1->display(m_score->readNumEntry(0, "Shots", 0));
    ShotLCD2->display(m_score->readNumEntry(1, "Shots", 0));
    ShotLCD3->display(m_score->readNumEntry(2, "Shots", 0));
    ShotLCD4->display(m_score->readNumEntry(3, "Shots", 0));
    ShotLCD5->display(m_score->readNumEntry(4, "Shots", 0));
    ShotLCD6->display(m_score->readNumEntry(5, "Shots", 0));
    ShotLCD7->display(m_score->readNumEntry(6, "Shots", 0));
    ShotLCD8->display(m_score->readNumEntry(7, "Shots", 0));
    ShotLCD9->display(m_score->readNumEntry(8, "Shots", 0));
    ShotLCD10->display(m_score->readNumEntry(9, "Shots", 0));

    HitLCD1->display(m_score->readNumEntry(0, "Hits", 0));
    HitLCD2->display(m_score->readNumEntry(1, "Hits", 0));
    HitLCD3->display(m_score->readNumEntry(2, "Hits", 0));
    HitLCD4->display(m_score->readNumEntry(3, "Hits", 0));
    HitLCD5->display(m_score->readNumEntry(4, "Hits", 0));
    HitLCD6->display(m_score->readNumEntry(5, "Hits", 0));
    HitLCD7->display(m_score->readNumEntry(6, "Hits", 0));
    HitLCD8->display(m_score->readNumEntry(7, "Hits", 0));
    HitLCD9->display(m_score->readNumEntry(8, "Hits", 0));
    HitLCD10->display(m_score->readNumEntry(9, "Hits", 0));

    WaterLCD1->display(m_score->readNumEntry(0, "Water", 0));
    WaterLCD2->display(m_score->readNumEntry(1, "Water", 0));
    WaterLCD3->display(m_score->readNumEntry(2, "Water", 0));
    WaterLCD4->display(m_score->readNumEntry(3, "Water", 0));
    WaterLCD5->display(m_score->readNumEntry(4, "Water", 0));
    WaterLCD6->display(m_score->readNumEntry(5, "Water", 0));
    WaterLCD7->display(m_score->readNumEntry(6, "Water", 0));
    WaterLCD8->display(m_score->readNumEntry(7, "Water", 0));
    WaterLCD9->display(m_score->readNumEntry(8, "Water", 0));
    WaterLCD10->display(m_score->readNumEntry(9, "Water", 0));
}

void KHighscoreDialog::save(const QString &name, int shots, int hits, int water)
{
    // Balancing factors
    // a = shot-balance
    // b = water-balance
    double a = 3;
    double b = 0.5;
    double score = (a * hits - b * water) / (shots + water) * 1000;
    
    if(score == 0)
	score = 1;
    
    int rank = -1;
    for(int i = 0; i <= 9; i++)
    {
	int oldscore = m_score->readNumEntry(i, "Score", 0);
	if(score >= 0)
	{
	    if(oldscore <= score)
	    {
		rank = i;
		break;
	    }
	}
	else
	{
    	    if(oldscore <= 0)
	    {
		if(score >= oldscore || oldscore == 0)
    		{
		    rank = i;
		    break;
		}
	    }
	}
    }
    
    if(rank == -1)
	return;

    for(int i = 9; i > rank; i--)
    {
	m_score->writeEntry(i, "Name", m_score->readEntry(i - 1, "Name", "-"));
	m_score->writeEntry(i, "Score", m_score->readNumEntry(i - 1, "Score", 0));
	m_score->writeEntry(i, "Shots", m_score->readNumEntry(i - 1, "Shots", 0));
	m_score->writeEntry(i, "Hits", m_score->readNumEntry(i - 1, "Hits", 0));
	m_score->writeEntry(i, "Water", m_score->readNumEntry(i - 1, "Water", 0));
    }
    
    m_score->writeEntry(rank, "Name", name);
    m_score->writeEntry(rank, "Score", score);
    m_score->writeEntry(rank, "Shots", shots);
    m_score->writeEntry(rank, "Hits", hits);
    m_score->writeEntry(rank, "Water", water);

    m_score->sync();
}
