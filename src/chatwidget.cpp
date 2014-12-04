/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "chatwidget.h"

#include <KLineEdit>
#include <KTextEdit>

#include <QVBoxLayout>
#include <QKeyEvent>

#include "entity.h"

ChatWidget::ChatWidget(QWidget* parent)
: QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);
    
    m_chat = new KTextEdit(this);
    m_chat->setReadOnly(true);
    layout->addWidget(m_chat);
    
    m_input = new KLineEdit(this);
    m_input->installEventFilter(this);
    layout->addWidget(m_input);
    
    setLayout(layout);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Maximum);
    
    m_chat->setFocusProxy(m_input);
    
    m_history.push_back("");
    m_current = 0;
    
    connect(m_input, &QLineEdit::returnPressed, this, &ChatWidget::sendLine);
}

void ChatWidget::setNick(const QString& nick)
{
    m_nick = nick;
}

void ChatWidget::setHistoryText(int index)
{
    m_history[m_current] = m_input->text();
    m_current = index;
    m_input->setText(m_history[m_current]);
}

bool ChatWidget::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == m_input && event->type() == QEvent::KeyPress) {
        QKeyEvent* e = (QKeyEvent*)event;
        if (e->key() == Qt::Key_Up) {
            if (m_current > 0) {
                setHistoryText(m_current - 1);
            }
        }
        else if (e->key() == Qt::Key_Down) {
            if (m_current < m_history.size() - 1) {
                setHistoryText(m_current + 1);
            }
        }
    }
    
    return false;
}

void ChatWidget::sendLine()
{
    QString text = m_input->text();
    m_history.push_back("");
    setHistoryText(m_history.size() - 1);
    display(m_nick, text);
    emit message(text);
}

void ChatWidget::display(const QString& nick, const QString& text)
{
    display('<' + nick + "> " + text);
}

void ChatWidget::display(const QString& text)
{
    if (isVisible()) {
        m_chat->append(text);
    }
}

void ChatWidget::bindTo(Entity* entity)
{
    connect(this, SIGNAL(message(QString)),
            entity, SIGNAL(chat(QString)));
    
    m_chat->clear();
    QWidget::show();
}

void ChatWidget::resizeEvent(QResizeEvent*)
{
}

QSize ChatWidget::sizeHint() const
{
    return QSize(100, 100);
}



