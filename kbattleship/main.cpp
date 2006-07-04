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
#include <kstandarddirs.h>
#include "kbattleship.h"

extern const char *clientName, *clientVersion, *clientDescription;

static KCmdLineOptions options[] =
{
	{"!+[URL]",                 I18N_NOOP("URL of server to attach to.  In the form kbattleship://host:port/ or host:port"), 0},
	KCmdLineLastOption
};

int main(int argc, char *argv[])
{
	KAboutData aboutData("kbattleship", clientName, clientVersion, clientDescription, KAboutData::License_GPL, "(c) 2000-2005  Nikolas Zimmermann, Daniel Molkentin");

	aboutData.addAuthor("Nikolas Zimmermann", I18N_NOOP("Project Founder, GUI Handling, Client/Server"), "wildfox@kde.org");
	aboutData.addAuthor("Daniel Molkentin", I18N_NOOP("Dialog Stuff, Client/Server"), "molkentin@kde.org");
	aboutData.addAuthor("Kevin Krammer", I18N_NOOP("Computer Player"), "kevin.krammer@gmx.at");
	aboutData.addCredit("Benjamin Adler", I18N_NOOP("Icon"), "benadler@bigfoot.de");
	aboutData.addCredit("Nils Trzebin", I18N_NOOP("Sounds"), "nils.trzebin@stud.uni-hannover.de");
	aboutData.addCredit("Elmar Hoefner", I18N_NOOP("GFX"), "elmar.hoefner@uibk.ac.at");
	aboutData.addCredit("Lukas Tinkl", I18N_NOOP("Non-Latin1 Support"), "lukas@kde.org");
	aboutData.addCredit("Malte Starostik", I18N_NOOP("Various improvements"), "malte.starostik@t-online.de");
	aboutData.addCredit("Albert Astals Cid", I18N_NOOP("Various improvements and bugfixes"), "tsdgeos@terra.es");
	aboutData.addCredit("John Tapsell", I18N_NOOP("Various improvements and bugfixes"), "john@geola.co.uk");
	aboutData.addCredit("Inge Wallin", I18N_NOOP("Bugfixes and refactoring"), "inge@lysator.liu.se");
	aboutData.addCredit("Jakub Stachowski", I18N_NOOP("DNS-SD discovery"), "qbast@go2.pl");

	KCmdLineArgs::init(argc, argv, &aboutData);
	KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.
	KApplication app;
	KGlobal::locale()->insertCatalog("libkdegames");

	QString picDirCheck = KStandardDirs::locate("data", "kbattleship/pictures/");
	QString onePicCheck = locate("data", "kbattleship/pictures/hit.png");
	if(picDirCheck.isEmpty() || onePicCheck.isEmpty())
	{
		KMessageBox::error(0, i18n("You don't have KBattleship pictures installed. The game cannot run without them!"));
		return 1;
	}
	
	if( app.isSessionRestored() )
		RESTORE(KBattleshipWindow)
	else {
		KBattleshipWindow *mainwin = new KBattleshipWindow;
		mainwin->show();
	}
	return app.exec();
}

