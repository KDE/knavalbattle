/***************************************************************************
                                   main.cpp
                                  ----------
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

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>

#include "kbattleship.h"

static const char *description =
	I18N_NOOP("KBattleship");
// INSERT A DESCRIPTION FOR YOUR APPLICATION HERE
	
	
static KCmdLineOptions options[] =
{
  { "+[File]", I18N_NOOP("file to open"), 0 },
  { 0, 0, 0 }
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};

int main(int argc, char *argv[])
{

	KAboutData aboutData( "kbattleship", I18N_NOOP("KBattleship"),
		VERSION, description, KAboutData::License_GPL,
		"(c) 2000, Daniel Molkentin, Nikolas Zimmermann");
	aboutData.addAuthor("Daniel Molkentin",0, "molkentin@kde.org");
	aboutData.addAuthor("Nikolas Zimmermann",0, "nikoz@gymnasium-kerpen.de");
	KCmdLineArgs::init( argc, argv, &aboutData );
	KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

  KApplication app;
 
  if (app.isRestored())
  {
    RESTORE(KBattleshipApp);
  }
  else 
  {
    KBattleshipApp *kbattleship = new KBattleshipApp();
    kbattleship->show();

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
		
		if (args->count())
		{
                        // do something
		}
		else
		{
                       // do something else
                }
		args->clear();
  }

  return app.exec();
}  
