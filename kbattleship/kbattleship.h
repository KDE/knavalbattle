/***************************************************************************
                                   kbattleship.h
                                  -----------------
    Developers: (c) 2000 Nikolas Zimmermann <nikoz@gymnasium-kerpen.de>
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

#ifndef KBATTLESHIP_H
#define KBATTLESHIP_H

#include <stdlib.h>
#include <kapp.h>
#include <kmainwindow.h>
#include <kaccel.h>
#include <kstdaction.h>
#include <kaction.h>
#include <qpainter.h>
#include <qobject.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>

#include "kbattleship.h"
#include "kbattleshipview.h"
#include "kserverdialog.h"
#include "kclientdialog.h"

class KBattleshipView;
class KServerDialog;

class KBattleshipApp : public KMainWindow
{
    Q_OBJECT

    public:
        KBattleshipApp( QWidget *parent = 0, const char *name = 0 );
        ~KBattleshipApp();
	
    protected:
        void saveOptions();
        void readOptions();
        void initActions();
        void initStatusBar();
        void initDocument();
        void initView();

    public slots:
	void slotServerConnect();
	void slotNewServer();
        void slotGameQuit();
        void slotViewToolBar();
        void slotViewStatusBar();
        void slotStatusMsg( const QString &text );
	void startBattleshipServer();

    private:
        KConfig *config;
        KBattleshipView *view;
	KAction *gameServerConnect;
	KAction *gameNewServer;
        KAction *gameQuit;
        KToggleAction *viewToolBar;
        KToggleAction *viewStatusBar;
        KClientDialog *client;
	KServerDialog *server;
};

#endif
