#include <QResizeEvent>
#include <QLayout>

#include "playfield.h"
#include "seaview.h"
#include "controller.h"

PlayField::PlayField(QWidget* parent)
: QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout;

    m_sea = new SeaView(this);
    m_controller = new OnePlayerController(this, m_sea, Sea::Player(0));
    m_sea->setController(m_controller);
    
    layout->addWidget(m_sea);
    
    setLayout(layout);
}


void PlayField::newGame()
{
    delete m_controller;
    m_controller = new OnePlayerController(this, m_sea, Sea::Player(0));
}

#include "playfield.moc"


