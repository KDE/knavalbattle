/*
  Copyright (c) 2000-2001 Nikolas Zimmermann <wildfox@kde.org>
            (c) 2000-2001 Daniel Molkentin <molkentin@kde.org>
            (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include <kapplication.h>
#include <klocale.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>

#include "mainwindow.h"
#include "coord.h"
#include "statswidget.h"

int main(int argc, char** argv)
{
    KAboutData aboutData("kbattleship", 0, ki18n("KBattleship"), "2.0", 
        ki18n("The KDE Battleship clone"), KAboutData::License_GPL, 
        ki18n("(c) 2000-2005  Nikolas Zimmermann, Daniel Molkentin\n"
              "(c) 2007 Paolo Capriotti"), KLocalizedString(), "http://games.kde.org/kbattleship" );
    
    aboutData.addAuthor(ki18n("Paolo Capriotti"), ki18n("Current maintainer"), "p.capriotti@gmail.com");
    aboutData.addAuthor(ki18n("Nikolas Zimmermann"), ki18n("Project Founder, GUI Handling, Client/Server"), "wildfox@kde.org");
    aboutData.addAuthor(ki18n("Daniel Molkentin"), ki18n("Dialog Stuff, Client/Server"), "molkentin@kde.org");
    aboutData.addAuthor(ki18n("Kevin Krammer"), ki18n("Computer Player"), "kevin.krammer@gmx.at");
    
    aboutData.addCredit(ki18n("Johann Ollivier Lapeyre"), ki18n("Game artwork"), "johann.ollivierlapeyre@gmail.com");
    aboutData.addCredit(ki18n("Eugene Trounev"), ki18n("Background"), "irs_me@hotmail.com");
    aboutData.addCredit(ki18n("Robert Wadley"), ki18n("Artwork for some of the ships"), "rob@robntina.fastmail.us");
    aboutData.addCredit(ki18n("Riccardo Iaconelli"), ki18n("Welcome screen"), "ruphy@fsfe.org");
    aboutData.addCredit(ki18n("Benjamin Adler"), ki18n("Icon"), "benadler@bigfoot.de");
    aboutData.addCredit(ki18n("Nils Trzebin"), ki18n("Sounds"), "nils.trzebin@stud.uni-hannover.de");
    aboutData.addCredit(ki18n("Elmar Hoefner"), ki18n("GFX"), "elmar.hoefner@uibk.ac.at");
    aboutData.addCredit(ki18n("Lukas Tinkl"), ki18n("Non-Latin1 Support"), "lukas@kde.org");
    aboutData.addCredit(ki18n("Malte Starostik"), ki18n("Various improvements"), "malte.starostik@t-online.de");
    aboutData.addCredit(ki18n("Albert Astals Cid"), ki18n("Various improvements and bugfixes"), "aacid@kde.org");
    aboutData.addCredit(ki18n("John Tapsell"), ki18n("Various improvements and bugfixes"), "john@geola.co.uk");
    aboutData.addCredit(ki18n("Inge Wallin"), ki18n("Bugfixes and refactoring"), "inge@lysator.liu.se");
    aboutData.addCredit(ki18n("Jakub Stachowski"), ki18n("DNS-SD discovery"), "qbast@go2.pl");
        
    KCmdLineArgs::init(argc, argv, &aboutData);

    KCmdLineOptions options;
    KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.
    KApplication app;
    
    KGlobal::locale()->insertCatalog("libkdegames");
    
    qRegisterMetaType<Coord>("Coord");
    
    MainWindow* window = new MainWindow();
//     StatsWidget* window = new StatsWidget(0, 0);
    window->show();
    
    return app.exec();
}
