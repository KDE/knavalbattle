#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#include <QWidget>

class SeaView;
class Controller;

class PlayField : public QWidget
{
    SeaView* m_sea;
    Controller* m_controller;
public:
    PlayField(QWidget* parent);
};

#endif // PLAYFIELD_H

