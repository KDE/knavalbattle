/***************************************************************************
                                  khighscoredialog.h
                                  -----------------
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

#ifndef KHIGHSCOREDIALOG_H
#define KHIGHSCOREDIALOG_H

#include <khighscore.h>
#include "dialogs/highscoreDlg.h"

class KHighscoreDialog : public highscoreDlg
{
	Q_OBJECT
public:
	KHighscoreDialog(QWidget *parent = 0, const char *name = 0);

	void load();
	void save(const QString &name, int shots, int hits, int water);

private:
	KHighscore *m_score;
};

#endif
