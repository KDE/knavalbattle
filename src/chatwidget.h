/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <QVector>

class QTextEdit;
class QLineEdit;
class QEvent;
class Entity;

class ChatWidget : public QWidget
{
Q_OBJECT
    QTextEdit* m_chat;
    QLineEdit* m_input;
    
    int m_current;
    QVector<QString> m_history;
    QString m_nick;
    
    void setHistoryText(int index);
public:
    explicit ChatWidget(QWidget* parent = nullptr);
    
    QSize sizeHint() const override;
    void bindTo(Entity* entity);

    void setNick(const QString& nick);
    void display(const QString& nick, const QString& message);
    void display(const QString& message);
protected:
    bool eventFilter(QObject*, QEvent* e) override;
    void resizeEvent(QResizeEvent*) override;
Q_SIGNALS:
    void message(const QString& text);
private Q_SLOTS:
    void sendLine();
};

#endif // CHATWIDGET_H
