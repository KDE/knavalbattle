/***************************************************************************
                                   kstatdialog.h
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

#ifndef KSTATDIALOG_H
#define KSTATDIALOG_H

#include "ui_statDlg.h"

class KStatDialog : public QWidget, public Ui::statDlg
{
    Q_OBJECT
public:
    explicit KStatDialog(QWidget *parent = 0);

    void setShot();
    void setShot(int shot);
    void setHit();
    void setHit(int hit);
    void setWater();
    void setWater(int water);

    void clear();
    void clearWon();

    int shot();
    int hit();
    int water();

public slots:
    void slotAddOwnWon();
    void slotAddEnemyWon();
};

#endif
