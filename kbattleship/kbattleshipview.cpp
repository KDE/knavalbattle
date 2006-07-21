/***************************************************************************
                             kbattleshipview.cpp
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

#include <QLayout>
#include <QMouseEvent>
#include <QKeyEvent>

#include <klocale.h>
#include <kdebug.h>
#include "kbattleship.h"
#include "kbattlefield.h"
#include "kshiplist.h"
#include "kmessage.h"
#include "kbattleshipview.moc"

KBattleshipView::KBattleshipView(QWidget *parent, bool draw) 
	: QWidget(parent, Qt::WResizeNoErase), m_drawGrid(draw)
{
	setFixedSize(20 * 32 + 30, 10 * 32 + 20);
	setMouseTracking(true);
	installEventFilter(this);

	m_decide = false;
	m_lastX = 0;
	m_lastY = 0;
	m_battlefield = 0;
}

KBattleshipView::~KBattleshipView()
{
	delete m_battlefield;
}

void KBattleshipView::startDrawing()
{
	m_battlefield = new KBattleField(this, m_drawGrid);
}

void KBattleshipView::clearField()
{
	m_battlefield->clearOwnField();
	m_battlefield->clearEnemyField();
}

void KBattleshipView::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	m_battlefield->drawField(p);
}

int KBattleshipView::ownFieldState(int fieldx, int fieldy)
{
	return m_battlefield->ownState(fieldx, fieldy);
}

int KBattleshipView::enemyFieldState(int &fieldx, int &fieldy)
{
	return m_battlefield->enemyState(fieldx, fieldy);
}

void KBattleshipView::previewShip(int fieldx, int fieldy, int type, bool rotate)
{
	m_battlefield->setPreviewState(fieldx, fieldy, type, rotate);
}

void KBattleshipView::changeOwnFieldData(int fieldx, int fieldy, int type)
{
	m_battlefield->setOwnState(fieldx, fieldy, type);
	update();
}

void KBattleshipView::changeEnemyFieldData(int fieldx, int fieldy, int type)
{
	m_battlefield->setEnemyState(fieldx, fieldy, type);
	update();
}

void KBattleshipView::drawEnemyShipsAI(KShipList *list)
{
	KShip *ship;
	int state;
	int grid = m_battlefield->gridSize();
	int width = m_battlefield->enemyRect().width() / grid;
	int height = m_battlefield->enemyRect().height() / grid;

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			ship = list->shipAt(i, j);
			state = enemyFieldState(i, j);
			if (ship && state != KBattleField::HIT && state != KBattleField::DEATH)
			{
				changeEnemyFieldData(i, j, ship->shipTypeEnum(i, j));
			}
		}
	}
}

void KBattleshipView::drawEnemyShipsHuman(KMessage *msg, KShipList *list)
{
	int posx, posy, placedLeft;
	bool left;
	int i = 3;
	while (!msg->field(QString("ship%1").arg(i)).isNull())
	{
		posx = msg->field(QString("ship%1").arg(i)).section(" ", 0, 0).toInt();
		posy = msg->field(QString("ship%1").arg(i)).section(" ", 1, 1).toInt();
		placedLeft = msg->field(QString("ship%1").arg(i)).section(" ", 2, 2).toInt();
		if (placedLeft == 0) left = false;
		else left = true;
		list->addNewShip(!left, posx, posy);
		i--;
	}
	drawEnemyShipsAI(list);
}

KMessage *KBattleshipView::getAliveShips(KShipList *list)
{
	KShip *ship;
	QString shipPos, shipNum;
	int shipType;
	int grid = m_battlefield->gridSize();
	int width = m_battlefield->enemyRect().width() / grid;
	int height = m_battlefield->enemyRect().height() / grid;
	KMessage *msg = new KMessage(KMessage::WON);
	bool shipsFound[4] = {false, false, false, false};

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			ship = list->shipAt(i, j);
			shipType = list->shipTypeAt(i, j);
			if (ship && !shipsFound[shipType])
			{
				shipPos.sprintf("%d %d %d", i, j, ship->placedLeft());
				shipsFound[shipType] = true;
				shipNum.sprintf("ship%d",shipType);
				msg->addField(shipNum, shipPos);
			}
		}
	}
	return msg;
}

bool KBattleshipView::eventFilter(QObject *object, QEvent *event)
{
	if(event->type() == QEvent::KeyPress && m_decide)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if(keyEvent->key() == Qt::Key_Shift){
			emit changeShipPlacementDirection();
			emit sigMouseOverField(m_lastX, m_lastY);
		}
	}
	else if(event->type() == QEvent::KeyRelease && m_decide)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if(keyEvent->key() == Qt::Key_Shift){
			emit changeShipPlacementDirection();
			emit sigMouseOverField(m_lastX, m_lastY);
		}
	}
	else if(event->type() == QEvent::MouseButtonRelease)
	{
		m_decide = false;

		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

		if(mouseEvent->button() == Qt::RightButton){
			emit changeShipPlacementDirection();
			emit sigMouseOverField(m_lastX, m_lastY);
			return true;
		}
		
		if(mouseEvent->button() != Qt::LeftButton)
			return false;

		QPoint point(mouseEvent->x(), mouseEvent->y());
		QRect ownRect = m_battlefield->ownRect();
		QRect enemyRect = m_battlefield->enemyRect();

		QRect newRect;

		int fieldx = 0;
		int fieldy = 0;

		if(ownRect.contains(point))
			newRect = ownRect;
		else if(enemyRect.contains(point))
			newRect = enemyRect;
		else
			return false;

		int j = -1;

		for(int i = newRect.left(); i <= newRect.right(); i += m_battlefield->gridSize())
		{
			j++;
			QRect tempRect(i, newRect.top(), m_battlefield->gridSize(), newRect.bottom() - newRect.top());

			if(tempRect.contains(point))
			{
				fieldx = j;
				break;
			}
		}

		j = -1;

		for(int i = newRect.top(); i <= newRect.bottom(); i += m_battlefield->gridSize())
		{
			j++;
			QRect tempRect(newRect.left(), i, newRect.right() - newRect.left(), m_battlefield->gridSize());

			if(tempRect.contains(point))
			{
				fieldy = j;
				break;
			}
		}

		if( newRect == ownRect)
			emit sigOwnFieldClicked(fieldx, fieldy);
		else if(newRect == enemyRect)
			emit sigEnemyFieldClicked(fieldx, fieldy);
		
		return true;
	}
	else if(event->type() == QEvent::MouseMove)
	{
		setFocus();
		m_decide = true;

		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

		QPoint point(mouseEvent->x(), mouseEvent->y());
		QRect ownRect = m_battlefield->ownRect();

		int fieldx = 0;
		int fieldy = 0;

		if(ownRect.contains(point))
		{
			int j = -1;

			for(int i = ownRect.left(); i <= ownRect.right(); i += m_battlefield->gridSize())
			{
				j++;
				QRect tempRect(i, ownRect.top(), m_battlefield->gridSize(), ownRect.bottom() - ownRect.top());

				if(tempRect.contains(point))
				{
					fieldx = j;
					break;
				}
			}

			j = -1;

			for(int i = ownRect.top(); i <= ownRect.bottom(); i += m_battlefield->gridSize())
			{
				j++;
				QRect tempRect(ownRect.left(), i, ownRect.right() - ownRect.left(), m_battlefield->gridSize());

				if(tempRect.contains(point))
				{
					fieldy = j;
					break;
				}
			}

			m_lastX = fieldx;
			m_lastY = fieldy;

			emit sigMouseOverField(fieldx, fieldy);
		}
		else
			update();

		return true;
	}

	return QWidget::eventFilter(object, event);
}
