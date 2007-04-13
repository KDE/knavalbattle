/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <QVector>

class QTextEdit;
class QLineEdit;
class QEvent;

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
    ChatWidget(QWidget* parent);
    
    void setNick(const QString& nick);
    virtual QSize sizeHint() const;
protected:
    virtual bool eventFilter(QObject*, QEvent* e);
    virtual void resizeEvent(QResizeEvent*);
signals:
    void message(const QString& nick, const QString& text);
public slots:
    void display(const QString& nick, const QString& message);
private slots:
    void sendLine();
};

#endif // CHATWIDGET_H
