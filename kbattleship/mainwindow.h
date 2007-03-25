#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <kmainwindow.h>

class PlayField;

class MainWindow : public KMainWindow
{
Q_OBJECT
    PlayField* m_main;
public:
    MainWindow();
protected:
    void setupActions();
protected slots:
    void optionsPreferences();
};

#endif // MAINWINDOW_H

