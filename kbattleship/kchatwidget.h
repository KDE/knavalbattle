/***************************************************************************
                              kchatwidget.h
                             ---------------
    Developers: (c) 2000 Nikolas Zimmermann <wildfox@kde.org>
                (c) 2000 Daniel Molkentin <molkentin@kde.org>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KCHATWIDGET_H
#define KCHATWIDGET_H

#include <chatwidget.h>
#include <qpushbutton.h>
#include <qlineedit.h>

class KChatWidget : public chatWidget
{
    Q_OBJECT

    public:
        KChatWidget( QWidget *parent = 0, const char *name = 0 );
        ~KChatWidget();

    public slots:
    void slotComputeMessage();

};

#endif
