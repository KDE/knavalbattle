/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "welcomescreen.h"

#include <kdebug.h>
#include <kicon.h>

#include "button.h"
#include "animator.h"

WelcomeScreen::WelcomeScreen(KGameCanvasAbstract* parent, const QFont& font)
: KGameCanvasGroup(parent)
, m_font(font)
, m_clicked(0)
, m_hover(0)
{
    m_background = new KGameCanvasRectangle(QColor(0, 0, 0, 80), m_size, this);
    m_background->show();
    
    Button* button;
    button = addButton(0, 0, KIcon("user-female"), "Human");
    connect(button, SIGNAL(clicked()), this, SIGNAL(human()));
    
    button = addButton(0, 1, KIcon("roll"), "Computer");
    connect(button, SIGNAL(clicked()), this, SIGNAL(ai()));
    
    button = addButton(0, 2, KIcon("network"), "Network");
    connect(button, SIGNAL(clicked()), this, SIGNAL(network()));
}

void WelcomeScreen::resize(const QSize& size)
{
    m_size = size;
    
    // background
    m_background->setSize(m_size);
    m_background->moveTo(0, 0);
    
    update();
}

void WelcomeScreen::update()
{
    kDebug() << "size = " << m_size << endl;

    // find dimensions
    int max_x = 0;
    int max_y = 0;
    for (Buttons::const_iterator i = m_buttons.constBegin();
         i != m_buttons.constEnd();
         ++i) {
        if (i.key().x > max_x) {
            max_x = i.key().x;
        }
        if (i.key().y > max_y) {
            max_y = i.key().y;
        }    
    }
    max_x++;
    max_y++;
    
    // place buttons
    QSize size = m_size;
    size.rwidth() /= max_x;
    size.rheight() /= max_y;
    
    for (Buttons::const_iterator i = m_buttons.constBegin();
         i != m_buttons.constEnd();
         ++i) {
        QPoint pos(size.width() * i.key().x,
                   size.height() * i.key().y);
        QPoint delta((size.width() - i.value()->size().width()) / 2,
                     (size.height() - i.value()->size().height()) / 2);
        i.value()->moveTo(pos + delta);
        
        pos += QPoint(size.width(), size.height());
    }
}

Button* WelcomeScreen::addButton(int x, int y, const QIcon& icon, const QString& text)
{
    if (m_buttons.contains(Coord(x, y))) {
        return 0;
    }
    else {
        Button* button = new Button(this, icon, m_font, text);
        m_buttons.insert(Coord(x, y), button);
        button->show();
        update();
        
        return button;
    }
}

void WelcomeScreen::onMouseMove(const QPoint& p)
{
    Button* button = dynamic_cast<Button*>(itemAt(p));
    if (m_hover && m_hover != button) {
        m_hover->onMouseLeave();
    }

    m_hover = button;
    if (button && (!m_clicked || button == m_clicked)) {
        button->onMouseMove(p - button->pos());
    }
}

void WelcomeScreen::onMousePress(const QPoint& p)
{
    Button* button = dynamic_cast<Button*>(itemAt(p));
    if (button) {
        button->onMousePress(p - button->pos());
        m_clicked = button;
    }
}

void WelcomeScreen::onMouseRelease(const QPoint& p)
{
    if (m_clicked) {
        m_clicked->onMouseRelease(p - m_clicked->pos());
    }

    Button* button = dynamic_cast<Button*>(itemAt(p));
    if (m_clicked && m_clicked == button) {
        // actual click event
        m_clicked->onClicked();
        
        Animation* hideAnimation = new FadeAnimation(this, opacity(), 0, 500);
        connect(hideAnimation, SIGNAL(done()), this, SLOT(hide()));
        Animator::instance()->add(hideAnimation);
    }
    
    m_clicked = 0;
}

void WelcomeScreen::hide()
{
    KGameCanvasGroup::hide();
}

void WelcomeScreen::onMouseLeave()
{
    if (m_hover) {
        m_hover->onMouseLeave();
    }
}


#include "welcomescreen.moc"

