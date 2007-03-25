#include <QResizeEvent>
#include <kstandardaction.h>
#include <kactioncollection.h>
#include <kaction.h>
#include <klocale.h>

#include "mainwindow.h"
#include "playfield.h"

MainWindow::MainWindow()
{
    m_main = new PlayField(this);
    setCentralWidget(m_main);
    setupActions();
    
    resize(900, 400);
}

void MainWindow::setupActions()
{
    KAction* temp = new KAction(i18n("&New game"), this);
    actionCollection()->addAction("game_singleplayer", temp);
    connect(temp, SIGNAL(triggered()), m_main, SLOT(newGame()));
    
    KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
    
    setupGUI();
}

void MainWindow::optionsPreferences()
{
    kDebug() << "preferences" << endl;
}

#include "mainwindow.moc"

