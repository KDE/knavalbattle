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
#include <qtooltip.h>
#include <qpainter.h>
#include <qwidget.h>
#include <qstring.h>
#include <qlist.h>
#include "kship.h"
#include "kbattlefield.h"

class KBattleshipViewToolTip : public QObject, public QToolTip
{
    Q_OBJECT
    public:
	KBattleshipViewToolTip(QWidget *parent, QRect own, QRect enemy) : QObject(parent), QToolTip(parent), ownRect(own), enemyRect(enemy) { set = true; }
	virtual ~KBattleshipViewToolTip() { }

    protected:
	virtual void maybeTip(const QPoint &point)
	{
	    if(ownRect.contains(point) && set)
		tip(QRect(point.x() - 5, point.y() - 5, point.x() + 5, point.y() + 5), i18n("Your Battlefield"));
	    else if(enemyRect.contains(point) && set)
		tip(QRect(point.x() - 5, point.y() - 5, point.x() + 5, point.y() + 5), i18n("Enemy's Battlefield"));
	    
	    if(set)
	    {
		set = false;
		emit activateTimer();
	    }
	}
	
    public slots:
	void slotTipTimeout() { set = true; }

    signals:
	void activateTimer();	
    
    private:    
	QRect ownRect;
	QRect enemyRect;
	
	bool set;
};

class KBattleshipView : public QWidget
{
    Q_OBJECT
    public:
        KBattleshipView(QWidget *parent = 0, const char *name = 0);
        ~KBattleshipView();

        void startDrawing();
        void clearField();
        void changeOwnFieldData(int fieldx, int fieldy, int type);
        void changeEnemyFieldData(int fieldx, int fieldy, int type);

	int getEnemyFieldState(int &fieldx, int &fieldy);

    private slots:
	void toolTipTimeout();
    
    signals:
        void enemyFieldClicked(int, int);
        void ownFieldClicked(int, int, int);

    protected:
        void paintEnemyField();
        void paintOwnField();
        void paintEvent(QPaintEvent *);
        void mouseReleaseEvent(QMouseEvent *event);

    private:
        KBattleField *ownfield;
        KBattleField *enemyfield;
	KBattleshipViewToolTip *toolTip;
};

#endif
