/***************************************************************************
                                   main.cpp
                                  ----------
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

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>

#include "kbattleship.h"

static const char *description = I18N_NOOP( "KBattleship" );

static const char *VERSION = I18N_NOOP( "0.1 (pre-alpha)" );

int main( int argc, char *argv[] )
{
    KAboutData aboutData( "kbattleship", I18N_NOOP( "KBattleship" ),
        VERSION, description, KAboutData::License_GPL,
        "(c) 2000  Nikolas Zimmermann, Daniel Molkentin" );

    aboutData.addAuthor( "Nikolas Zimmermann", "Project Founder, GUI Handling, Client/Server", "wildfox@kde.org" );
    aboutData.addAuthor( "Daniel Molkentin", "Dialog Stuff, Client/Server", "molkentin@kde.org" );
    aboutData.addCredit( "Benjamin Adler", "Icon", "benadler@bigfoot.de" );
    aboutData.addCredit( "Nils Trzebin", "Sounds", "nils.trzebin@stud.uni-hannover.de" );
	aboutData.addCredit( "Agnieszka Czajkowska", "GFX", "agnieszka@imagegalaxy.de" );

    KCmdLineArgs::init( argc, argv, &aboutData );
    KApplication app;
 
    if( app.isRestored() )
    {
        RESTORE( KBattleshipApp );
    }
    else
    {
        KBattleshipApp *kbattleship = new KBattleshipApp();
		kbattleship->init();
        kbattleship->show();
    }

    return app.exec();
}  
