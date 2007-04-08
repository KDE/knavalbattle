/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef STREAMREADER_H
#define STREAMREADER_H

#include <QString>
#include "message.h"

class QIODevice;

class Protocol : public QObject
{
Q_OBJECT
    QIODevice* m_device;
    QString m_buffer;
    
    MessagePtr parseMessage(const QString& xmlMessage);
public:
    explicit Protocol(QIODevice* device);
    
    void send(const MessagePtr& msg);
private slots:
    void readMore();
signals:
    void received(MessagePtr);
    void parseError(const QString&);
};


#endif // STREAMREADER_H
