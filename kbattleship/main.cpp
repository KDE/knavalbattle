/***************************************************************************
                                   main.cpp
                                  ----------
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

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>
//#include <dcopclient.h>

#include "kbattleship.h"
#include "main.h"

int main(int argc, char *argv[])
{
	KAboutData aboutData("kbattleship", clientName, clientVersion, clientDescription, KAboutData::License_GPL, "(c) 2000-2001  Nikolas Zimmermann, Daniel Molkentin");

	aboutData.addAuthor("Nikolas Zimmermann", I18N_NOOP("Project Founder, GUI Handling, Client/Server"), "wildfox@kde.org");
	aboutData.addAuthor("Daniel Molkentin", I18N_NOOP("Dialog Stuff, Client/Server"), "molkentin@kde.org");
	aboutData.addAuthor("Kevin Krammer", I18N_NOOP("Computer Player"), "kevin.krammer@gmx.at");
	aboutData.addCredit("Benjamin Adler", I18N_NOOP("Icon"), "benadler@bigfoot.de");
	aboutData.addCredit("Nils Trzebin", I18N_NOOP("Sounds"), "nils.trzebin@stud.uni-hannover.de");
	aboutData.addCredit("Elmar Hoefner", I18N_NOOP("GFX"), "elmar.hoefner@uibk.ac.at");
	aboutData.addCredit("Lukas Tinkl", I18N_NOOP("Non-Latin1 Support"), "lukas@kde.org");
	aboutData.addCredit("Malte Starostik", I18N_NOOP("Various improvements"), "malte.starostik@t-online.de");

	KCmdLineArgs::init(argc, argv, &aboutData);
	KApplication app;

	//(kapp->dcopClient())->attach();
	//(kapp->dcopClient())->registerAs("kbattleship");

	if(app.isRestored())
	{
		RESTORE(KBattleshipApp)
	}
	else
	{
		KBattleshipApp *kbattleship = new KBattleshipApp();
		kbattleship->show();
	}

	return app.exec();
}  
