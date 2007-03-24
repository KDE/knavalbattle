#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <kmainwindow.h>

class SeaView;
class Controller;

class MainWindow : public KMainWindow
{
    SeaView* m_sea;
    Controller* m_controller;
public:
    MainWindow();
protected:
    virtual void resizeEvent(QResizeEvent*);
};

#endif // MAINWINDOW_H

