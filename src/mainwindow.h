/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>
    SPDX-FileCopyrightText: 2007 Riccardo Iaconelli <ruphy@fsfe.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>

class PlayField;

class QUrl;

class MainWindow : public KXmlGuiWindow
{
Q_OBJECT
    PlayField* m_main;
public:
    explicit MainWindow(const QUrl& url);
protected:
    void setupActions();
private Q_SLOTS:
    void startingGame();
    void welcomeScreen();
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H

