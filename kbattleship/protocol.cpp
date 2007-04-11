/*
  Copyright (c) 2000-2001 Nikolas Zimmermann <wildfox@kde.org>
            (c) 2000-2001 Daniel Molkentin <molkentin@kde.org>
            (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include <QStringList>
#include <QDomElement>
#include <QDomNode>

#include "protocol.h"

#define ADD_FIELD(msg, field) addField(#field, msg.field())
class MessageSender : public MessageVisitor
{
    QDomDocument m_doc;
    QDomElement m_main;
    
    void addField(const QString& key, const QString& value)
    {
        QDomElement element = m_doc.createElement(key);
        QDomText text = m_doc.createTextNode(value);
        element.appendChild(text);
        m_main.appendChild(element);
    }
    
    template <typename Msg>
    void setType(const Msg&)
    {
        addField("msgtype", QString::number(Msg::MSGTYPE));
    }
public:
    MessageSender()
    : m_doc("kmessage")
    {
        m_main = m_doc.createElement("kmessage");
        m_doc.appendChild(m_main);
    }
    
    QDomDocument document() { return m_doc; }

    virtual void visit(const HeaderMessage& msg)
    {
        setType(msg);
        ADD_FIELD(msg, protocolVersion);
        ADD_FIELD(msg, clientName);
        ADD_FIELD(msg, clientVersion);
        ADD_FIELD(msg, clientDescription);
    }
    
    virtual void visit(const RejectMessage& msg) { setType(msg); }
    
    virtual void visit(const NickMessage& msg)
    {
        setType(msg);
        ADD_FIELD(msg, nickname);
    }
    
    virtual void visit(const BeginMessage& msg) { setType(msg); }
    
    virtual void visit(const MoveMessage& msg)
    {
        setType(msg);
        addField("fieldx", QString::number(msg.move().x));
        addField("fieldy", QString::number(msg.move().y));
    }
    
    virtual void visit(const NotificationMessage& msg)
    {
        setType(msg);
        addField("fieldx", QString::number(msg.move().x));
        addField("fieldy", QString::number(msg.move().y));
        addField("fieldstate", msg.hit() ? "1" : "99");
        if (msg.death()) {
            addField("death", "true");
            addField("xstart", QString::number(msg.start().x));
            addField("xstop", QString::number(msg.stop().x));
            addField("ystart", QString::number(msg.start().y));
            addField("ystop", QString::number(msg.stop().y));
        }
    }
    
    virtual void visit(const GameOverMessage& msg)
    {
        setType(msg);
        foreach (GameOverMessage::ShipInfo ship, msg.ships()) {
            QStringList data;
            data << QString::number(ship.pos.x)
                 << QString::number(ship.pos.y)
                 << (ship.direction == Ship::TOP_DOWN ? "0" : "1");
            addField(QString("ship") + QString::number(ship.size), data.join(" "));
        }
    }
    
    virtual void visit(const RestartMessage& msg)
    {
        setType(msg);
    }
    
    virtual void visit(const ChatMessage& msg)
    {
        setType(msg);
        ADD_FIELD(msg, chat);
    }
};



Protocol::Protocol(QIODevice* device)
: m_device(device)
{
    m_device->setParent(this);
    connect(m_device, SIGNAL(readyRead()), this, SLOT(readMore()));
}

void Protocol::readMore()
{
    QByteArray data = m_device->readAll();
    m_buffer += QString::fromUtf8(data.constData());
    
    int pos;
    while ((pos = m_buffer.indexOf("</kmessage>")) >= 0) {
        pos += 11; // Length of "</kmessage>"
        MessagePtr msg = parseMessage(m_buffer.left(pos));
        m_buffer.remove(0, pos);
        
        emit received(msg);
    }
}

#define DEF_ELEMENT(var) QString var = main.namedItem(#var).toElement().text()
#define DEF_COORD(var, varx, vary) DEF_ELEMENT(varx); DEF_ELEMENT(vary); Coord var(varx.toInt(), vary.toInt());
MessagePtr Protocol::parseMessage(const QString& xmlMessage)
{
    kDebug() << "received: " << xmlMessage << endl;

    QDomDocument doc;
    doc.setContent(xmlMessage);
    
    QDomElement main = doc.documentElement();
    if (main.tagName() != "kmessage")
    {
        emit parseError("Invalid parent tag");
        return MessagePtr();
    }
    
    QDomElement msgtype = main.namedItem("msgtype").toElement();
    if (msgtype.isNull())
    {
        emit parseError("No message type");
        return MessagePtr();
    }
    
    int type = msgtype.text().toInt();
    switch (type) {
    case HeaderMessage::MSGTYPE:
        {
            DEF_ELEMENT(protocolVersion);
            DEF_ELEMENT(clientName);
            DEF_ELEMENT(clientVersion);
            DEF_ELEMENT(clientDescription);
            return MessagePtr(new HeaderMessage(protocolVersion, clientName,
                                     clientVersion, clientDescription));
        }
    case RejectMessage::MSGTYPE:
        {
            DEF_ELEMENT(kmversion);
            DEF_ELEMENT(reason);
            return MessagePtr(new RejectMessage(kmversion == "true", reason));
        }
    case NickMessage::MSGTYPE:
        {
            DEF_ELEMENT(nickname);
            return MessagePtr(new NickMessage(nickname));
        }
    case BeginMessage::MSGTYPE:
        return MessagePtr(new BeginMessage);
    case MoveMessage::MSGTYPE:
        {
            DEF_COORD(field, fieldx, fieldy);
            return MessagePtr(new MoveMessage(field));
        }
    case NotificationMessage::MSGTYPE:
        {
            DEF_COORD(field, fieldx, fieldy);
            DEF_ELEMENT(fieldstate);
            bool hit = fieldstate == "1";
            DEF_ELEMENT(death);
            bool destroyed = death == "true";
            if (destroyed) {
                DEF_COORD(start, xstart, ystart);
                DEF_COORD(stop, xstop, ystop);
                return MessagePtr(new NotificationMessage(field, hit, destroyed, start, stop));
            }
            else {
                return MessagePtr(new NotificationMessage(field, hit, destroyed));
            }
        }
    case GameOverMessage::MSGTYPE:
        {
            GameOverMessage* msg = new GameOverMessage;
            QDomNodeList nodes = main.childNodes();
            for (int i = 0; i < nodes.count(); i++) {
                QDomElement element = nodes.item(i).toElement();
                if (!element.isNull() && element.tagName().startsWith("ship")) {
                    int size = element.tagName().mid(4).toInt() + 1;
                    QStringList data = element.text().split(" ");
                    if (data.size() != 3) {
                        continue;
                    }
                    Coord pos(data[0].toInt(), data[1].toInt());
                    Ship::Direction direction = data[2] == "0"
                        ? Ship::TOP_DOWN 
                        : Ship::LEFT_TO_RIGHT;
                    msg->addShip(pos, size, direction);
                }
            }
            
            return MessagePtr(msg);
        }
    case RestartMessage::MSGTYPE:
        return MessagePtr(new RestartMessage());
    case ChatMessage::MSGTYPE:
        {
            DEF_ELEMENT(chat);
            return MessagePtr(new ChatMessage(chat));
        }
    default:
        emit parseError("Unknown message type");
        return MessagePtr();
    }
}
#undef DEF_COORD
#undef DEF_ELEMENT

void Protocol::send(const Message& msg)
{
    MessageSender sender;
    msg.accept(sender);
    
    QTextStream stream(m_device);
    stream << sender.document().toString();
    
    kDebug() << "sending: " << sender.document().toString() << endl;
}


#include "protocol.moc"


