/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <QHash>
#include <QFont>
#include <QGraphicsRectItem>

#include "grid.h"

class Button;

class WelcomeScreen : public QObject, public QGraphicsRectItem 
{
Q_OBJECT
    typedef QHash<Coord, Button*> Buttons;
    Buttons m_buttons;
    QFont m_font;
    QSizeF m_size;
    
    Button* m_clicked;
    Button* m_hover;
    bool m_active;
public:
    explicit WelcomeScreen(const QFont& font);
    
    void resize(const QSizeF&);
    
    Button* addButton(int x, int y, const QIcon& icon, const QString& text);
    void removeButton(int x, int y);
    void moveButton(int x1, int y1, int x2, int y2);
    void clearButtons();
    
    void onMousePress(Button *button);
    void onMouseRelease(Button *button);
    void onMouseMove(Button *button);
    void onMouseLeave();
    
    bool active() const { return m_active; }
    void show();
    
public Q_SLOTS:
    void hide();
    void fadeOut();
    void refresh();
    
Q_SIGNALS:
    void clicked(Button*);
    void shown();
    void hidden();
};

#endif // WELCOMESCREEN_H
