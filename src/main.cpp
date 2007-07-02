/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include <kapplication.h>
#include <klocale.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <QMetaType>

#include "mainwindow.h"
#include "coord.h"
#include "statswidget.h"

int main(int argc, char** argv)
{
    KAboutData aboutData("kbattleship", 0, ki18n("KBattleship"), "2.0", ki18n("The KDE Battleship clone"), KAboutData::License_GPL, ki18n("(c) 2007  Paolo Capriotti"));
    
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
