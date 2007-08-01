/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "button.h"

#include <QImage>
#include <QLineEdit>
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
, m_fixed_width(false)
, m_down(false)
, m_hover(false)
, m_brightness(BRIGHTNESS_NORMAL)
, m_editor(0)
{
    computeSize();
    repaint();
}

Button::~Button()
{
    if (m_animation) {
        m_animation->abort();
    }
    delete m_editor;
}

void Button::setWidth(int width)
{
    m_fixed_width = width != -1;
    m_size.setWidth(width);
    computeSize();
}

void Button::computeSize()
{
    QFontMetrics fm(m_font);
    m_text_width = fm.width(m_text);
    int h = fm.height();
    if (h < 32) {
        h = 32;
    }
    if (!m_fixed_width) {        
        m_size = QSize(m_text_width, h);
        m_size.rwidth() += 10 + 32 + 10 + 10;
    }
    else {
        m_size.setHeight(h);
    }
    
    m_size.rheight() += 10 + 10;
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
        p.drawRoundRect(1, 1, m_size.width() - 2, m_size.height() -2, 
            2000 / m_size.width(), 2000 / m_size.height());
        p.drawPixmap(10, 
                    m_size.height() / 2 - 16, 
                    32, 
                    32, 
                    m_icon.pixmap(32, 32));
    
        if (!m_editor) {
            p.setFont(m_font);
            p.drawText(textPos(), m_text);
        }
    }
    
    setPixmap(QPixmap::fromImage(tmp));
    updateEditor();
}

QSize Button::size() const
{
    return m_size;
}

QPoint Button::textPos() const
{
    return QPoint(32 + 10 + (m_size.width() - 32 - 10 - m_text_width) / 2, 
        m_size.height() / 2 + 6);
}

void Button::onMousePress(const QPoint&)
{
    if (!m_editor && !m_down) {
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
    if (!m_editor && m_down) {
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
    if (!m_editor && !m_hover) {
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

bool Button::onClicked()
{
    if (!m_editor) {
        kDebug() << "clicked";
        emit clicked();
        return true;
    }
    else {
        return false;
    }
}

void Button::setText(const QString& text)
{
    m_text = text;
    repaint();
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

void Button::setEditor(EditorFactory& factory)
{
    // remove old editor if existent
    delete m_editor;
    
    // create a new editor
    m_editor = factory.createEditor(topLevelCanvas());
    
    // update button
    m_size.setWidth(32 * 6);
    emit needsUpdate();
    repaint();
    m_editor->show();
    
    m_editor->setFocus();
}

void Button::updateEditor()
{
    if (m_editor) {
        const int SPACE = 9;
        m_editor->setGeometry(QRect(absolutePosition() + QPoint(textPos().x(), SPACE),
            QSize(m_size.width() - 10 - 10 - 32 - 10, m_size.height() - SPACE * 2)));
    }
}

void Button::removeEditor()
{
    m_editor->hide();
    m_editor->deleteLater();
    m_editor = 0;
    
    computeSize();
    repaint();
    emit needsUpdate();
}

// ------------

double ButtonAnimation::m_speed = 0.46;

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
//     kDebug() << "button step t =" << t << "sign =" << sign <<  "delta =" << delta;
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

