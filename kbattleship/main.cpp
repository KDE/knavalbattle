#include <kapplication.h>
#include <klocale.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>

#include "mainwindow.h"

static KCmdLineOptions options[] = {
    KCmdLineLastOption
};

int main(int argc, char** argv)
{
    KAboutData aboutData("kbattleship", I18N_NOOP("KBattleShip"), "2.0", I18N_NOOP("The KDE Battleship clone"), KAboutData::License_GPL, "(c) 2007  Paolo Capriotti");
    
    KCmdLineArgs::init(argc, argv, &aboutData);
    KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.
    KApplication app;
    
    KGlobal::locale()->insertCatalog("libkdegames");
    
    MainWindow* window = new MainWindow;
    window->show();
    
    return app.exec();
}
