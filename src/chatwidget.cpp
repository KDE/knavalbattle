/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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
    layout->setContentsMargins(0, 0, 0, 0);
    
    m_chat = new KTextEdit(this);
    m_chat->setReadOnly(true);
    layout->addWidget(m_chat);
    
    m_input = new KLineEdit(this);
    m_input->installEventFilter(this);
    layout->addWidget(m_input);
    
    setLayout(layout);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Maximum);
    
    m_chat->setFocusProxy(m_input);
    
    m_history.push_back(QString());
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
    m_history.push_back(QString());
    setHistoryText(m_history.size() - 1);
    display(m_nick, text);
    Q_EMIT message(text);
}

void ChatWidget::display(const QString& nick, const QString& text)
{
    display(QLatin1Char('<') + nick + QLatin1String("> ") + text);
}

void ChatWidget::display(const QString& text)
{
    if (isVisible()) {
        m_chat->append(text);
    }
}

void ChatWidget::bindTo(Entity* entity)
{
    connect(this, &ChatWidget::message, entity, &Entity::chat);
    
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

#include "moc_chatwidget.cpp"
