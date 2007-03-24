#include <QResizeEvent>

#include "mainwindow.h"
#include "seaview.h"
#include "controller.h"

MainWindow::MainWindow()
{
    m_sea = new SeaView(this);
    m_controller = new OnePlayerController(this, m_sea, Sea::Player(0));
    m_sea->setController(m_controller);
    resize(900, 400);
}

void MainWindow::resizeEvent(QResizeEvent* e)
{
    m_sea->resize(e->size());
}

