/***************************************************************************
                              kbattleshipview.h
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

#ifndef KBATTLESHIPVIEW_H
#define KBATTLESHIPVIEW_H

#include <kmainwindow.h>

#include <qpainter.h>
#include <qptrlist.h>
#include <qstring.h>
#include <qwidget.h>

#include "kbattlefield.h"
#include "kmessage.h"
#include "kship.h"
#include "kshiplist.h"

class KBattleshipView : public QWidget
{
	Q_OBJECT
public:
	KBattleshipView(QWidget *parent = 0, const char *name = 0, bool draw = false);
	~KBattleshipView();

	KBattleField *field() { return m_battlefield; }

	void startDrawing();
	void clearField();
	void changeOwnFieldData(int fieldx, int fieldy, int type);
	void changeEnemyFieldData(int fieldx, int fieldy, int type);

	void previewShip(int fieldx, int fieldy, int type, bool rotate);

	int ownFieldState(int fieldx, int fieldy);
	int enemyFieldState(int &fieldx, int &fieldy);

	void drawEnemyShipsAI(KShipList *list);
	void drawEnemyShipsHuman(KMessage *msg, KShipList *list);
	KMessage *getAliveShips(KShipList *list);

signals:
	void sigEnemyFieldClicked(int, int);
	void sigOwnFieldClicked(int, int);
	void sigMouseOverField(int, int);
	void changeShipPlacementDirection();

private:
	bool eventFilter(QObject *object, QEvent *event);

	KBattleField *m_battlefield;
	bool m_drawGrid;
	bool m_decide;
	int m_lastX;
	int m_lastY;
};

#endif
