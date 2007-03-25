#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#include <QWidget>

class SeaView;
class Controller;

class PlayField : public QWidget
{
Q_OBJECT
    SeaView* m_sea;
    Controller* m_controller;
public:
    PlayField(QWidget* parent);
public slots:
    void newGame();
};

#endif // PLAYFIELD_H

