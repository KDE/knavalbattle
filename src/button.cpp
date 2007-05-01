/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "button.h"

#include <QImage>
#include <kdebug.h>
#include <math.h> // fabs

#include "animator.h"

Button::Button(WelcomeScreen* parent, const QIcon& icon, 
               const QFont& font, const QString& text)
: QObject(parent)
, KGameCanvasPixmap(parent)
, m_icon(icon)
, m_font(font)
, m_text(text)
, m_down(false)
, m_hover(false)
, m_brightness(BRIGHTNESS_NORMAL)
{
    QFontMetrics fm(m_font);
    int h = fm.height();
    if (h < 32) {
        h = 32;
    }
    m_size = QSize(fm.width(m_text), h);
    m_size.rwidth() += 10 + 32 + 10 + 10;
    m_size.rheight() += 10 + 10;
    
    repaint();
}

void Button::repaint()
{
    QImage tmp(m_size, QImage::Format_ARGB32_Premultiplied);
    tmp.fill(0);
    {
        QPainter p(&tmp);
        p.setRenderHint(QPainter::Antialiasing);
        QPen pen(QColor(200, 200, 220, 255));
        pen.setWidth(2);
        p.setPen(pen);
        p.setBrush(QBrush(QColor(
            static_cast<int>(m_brightness),
            static_cast<int>(m_brightness), 
            static_cast<int>(m_brightness), 100)));
        p.drawRoundRect(1, 1, m_size.width() - 2, m_size.height() -2, 8, 40);
        p.drawPixmap(10, 
                    m_size.height() / 2 - 16, 
                    32, 
                    32, 
                    m_icon.pixmap(32, 32));
    
        p.setFont(m_font);
        p.drawText(10+32+10, 
                m_size.height() / 2 + 6, 
                m_text);
    }
    
    setPixmap(QPixmap::fromImage(tmp));
}

QSize Button::size() const
{
    return m_size;
}

void Button::onMousePress(const QPoint&)
{
    if (!m_down) {
        m_down = true;
        if (m_animation) {
            m_animation->abort();
        }
        m_brightness = BRIGHTNESS_DOWN;
        
        repaint();
    }
}

void Button::onMouseRelease(const QPoint&)
{
    if (m_down) {
        m_down = false;
        if (m_animation) {
            m_animation->abort();
        }
        m_brightness = BRIGHTNESS_NORMAL;
        repaint();
    }
}

void Button::onMouseMove(const QPoint&)
{
    if (!m_hover) {
        m_hover = true;
        
        if (m_down) {
            if (m_animation) {
                m_animation->abort();
            }
            m_brightness = BRIGHTNESS_HOVER;
        }
        else if (m_animation) {
            m_animation->setBrightness(BRIGHTNESS_HOVER);
        }
        else {
            m_animation = new ButtonAnimation(this, BRIGHTNESS_HOVER);
            Animator::instance()->add(m_animation);
        }
        
        repaint();
    }
}

void Button::onMouseLeave()
{
    if (m_hover) {
        m_hover = false;
        
        if (m_down) {
            if (m_animation) {
                m_animation->abort();
            }
            m_brightness = BRIGHTNESS_NORMAL;
        }
        else if (m_animation) {
            m_animation->setBrightness(BRIGHTNESS_NORMAL);
        }
        else {
            m_animation = new ButtonAnimation(this, BRIGHTNESS_NORMAL);
            Animator::instance()->add(m_animation);
        }
        
        repaint();
    }
}

void Button::onClicked()
{
    kDebug() << "clicked" << endl;
    emit clicked();
}

double Button::brightness() const
{
    return m_brightness;
}

void Button::setBrightness(double value)
{
    m_brightness = value;
    repaint();
}

KGameCanvasPixmap* Button::extractIcon()
{
    KGameCanvasPixmap* res = new KGameCanvasPixmap(
        KGameCanvasPixmap::canvas());
    res->moveTo(pos() + QPoint(10, 10));
    res->setPixmap(m_icon.pixmap(32, 32));
    
    m_icon = QIcon();
    repaint();
    
    return res;
}


// ------------

double ButtonAnimation::m_speed = 0.38;

ButtonAnimation::ButtonAnimation(Button* button, int brightness)
: m_button(button)
, m_brightness(brightness)
{
    m_last = -1;
}

void ButtonAnimation::start(int t)
{
    m_last = t;
}

bool ButtonAnimation::step(int t)
{
    if (m_last == -1) {
        return true;
    }
    
    int sign = (m_button->brightness() > m_brightness) ? -1 : 1;
    double delta = (t - m_last) * m_speed;
    m_last = t;
    if (fabs(m_button->brightness() - m_brightness) <= delta) {
        m_button->setBrightness(m_brightness);
        return true;
    }
    else {
        m_button->setBrightness(m_button->brightness() + sign * delta);
        return false;
    }
}

void ButtonAnimation::abort()
{
    m_last = -1;
}

void ButtonAnimation::setBrightness(int value)
{
    m_brightness = value;
}

ButtonAnimation::~ButtonAnimation()
{
}


#include "button.moc"

