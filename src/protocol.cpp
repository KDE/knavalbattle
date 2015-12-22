/*
  Copyright (c) 2000-2001 Nikolas Zimmermann <wildfox@kde.org>
            (c) 2000-2001 Daniel Molkentin <molkentin@kde.org>
            (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "protocol.h"

#include <QStringList>
#include <QDomElement>
#include <QDomNode>
#include <QTcpSocket>

#define ADD_FIELD(msg, field) addField(#field, msg.field())
class MessageSender : public MessageVisitor
{
    QDomDocument m_doc;
    QDomElement m_main;

    QDomElement addField(const QString& key, const QString& value)
    {
        QDomElement element = m_doc.createElement(key);
        QDomText text = m_doc.createTextNode(value);
        element.appendChild(text);
        m_main.appendChild(element);
        return element;
    }

    template <typename Msg>
    void setType(const Msg&)
    {
        QDomElement element = addField("msgtype", QString::number(Msg::MSGTYPE));
        // only useful for debugging, just add the name of the message type
        element.setAttribute(QLatin1String("type"), Msg::messageType());
    }
public:
    MessageSender()
    : m_doc("kmessage")
    {
        m_main = m_doc.createElement("kmessage");
        m_doc.appendChild(m_main);
    }
    
    QDomDocument document() const { return m_doc; }

    void visit(const HeaderMessage& msg) override
    {
        setType(msg);
        ADD_FIELD(msg, protocolVersion);
        ADD_FIELD(msg, clientName);
        ADD_FIELD(msg, clientVersion);
        ADD_FIELD(msg, clientDescription);
    }
    
    void visit(const RejectMessage& msg) override { setType(msg); }
    
    void visit(const NickMessage& msg) override
    {
        setType(msg);
        ADD_FIELD(msg, nickname);
    }
    
    void visit(const BeginMessage& msg) override { setType(msg); }
    
    void visit(const MoveMessage& msg) override
    {
        setType(msg);
        addField("fieldx", QString::number(msg.move().x));
        addField("fieldy", QString::number(msg.move().y));
    }
    
    void visit(const NotificationMessage& msg) override
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
    
    void visit(const GameOverMessage& msg) override
    {
        setType(msg);
        foreach (const GameOverMessage::ShipInfo &ship, msg.ships()) {
            QStringList data;
            data << QString::number(ship.pos.x)
                 << QString::number(ship.pos.y)
                 << (ship.direction == Ship::TOP_DOWN ? "0" : "1");
            addField(QString("ship") + QString::number(ship.size), data.join( QLatin1String( " " )));
        }
    }
    
    void visit(const RestartMessage& msg) override
    {
        setType(msg);
    }
    
    void visit(const ChatMessage& msg) override
    {
        setType(msg);
        ADD_FIELD(msg, chat);
        ADD_FIELD(msg, nickname);
    }

    void visit(const GameOptionsMessage& msg) override
    {
        // create the message XML contents
        setType(msg);
        ADD_FIELD(msg, enabledAdjacentShips);
        QDomElement oneOrElement=addField ("oneOrSeveralShips", QString(msg.oneOrSeveralShips()));
        oneOrElement.setAttribute(QLatin1String("longestShip"),QString::number(msg.shipsConfiguration()->longestShip()));
        addField("boardWidth", QString::number(msg.gridWidth()));
        addField("boardHeight", QString::number(msg.gridHeight()));
        for (unsigned int i=1; i<=msg.shipsConfiguration()->longestShip(); i++) {
            QDomElement element=addField(QString("ships"), QLatin1String( "" ));
            element.setAttribute(QLatin1String("size"),QString::number(i));
            element.setAttribute(QLatin1String("number"),QString::number(msg.shipsConfiguration()->numberOfShipsOfSize(i)));
            element.setAttribute(QLatin1String("name"),msg.shipsConfiguration()->nameOfShipsOfSize(i));
            element.setAttribute(QLatin1String("pluralName"),msg.shipsConfiguration()->pluralNameOfShipsOfSize(i));
        }
    }
};



Protocol::Protocol(QTcpSocket* device)
: m_device(device)
{
    m_device->setParent(this);
    m_timer.start(100);
    connect(m_device, &QTcpSocket::disconnected, this, &Protocol::processDisconnection);
    connect(m_device, &QTcpSocket::readyRead, this, &Protocol::readMore);
    connect(&m_timer, &QTimer::timeout, this, &Protocol::sendNext);
}

void Protocol::readMore()
{
    QByteArray data = m_device->read(4096);
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
    qDebug() << "received:" << xmlMessage;

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
            bool hit = fieldstate != "99";
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
            GameOverMessage* msg = new GameOverMessage();
            QDomNodeList nodes = main.childNodes();
            for (int i = 0; i < nodes.count(); i++) {
                QDomElement element = nodes.item(i).toElement();
                if (!element.isNull() && element.tagName().startsWith("ship")) {
                    int size = element.tagName().mid(4).toInt();
                    QStringList data = element.text().split(' ');
                    if (data.size() != 3) {
                        continue;
                    }
                    Coord pos(data[0].toInt(), data[1].toInt());
                    Ship::Direction direction = data[2] == QChar('0')
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
            DEF_ELEMENT(nickname);
            DEF_ELEMENT(chat);
            return MessagePtr(new ChatMessage(nickname, chat));
        }
    case GameOptionsMessage::MSGTYPE:
        {
            // get values from the xml message
            DEF_ELEMENT(enabledAdjacentShips);
            bool adjacentShips = enabledAdjacentShips=="true";
            QDomElement oneOrElement=main.namedItem(QLatin1String("oneOrSeveralShips")).toElement();
            QString oneOrSeveralShips = oneOrElement.text();
            bool severalShips = oneOrSeveralShips=="true";
            unsigned int longestShip=0;
            // if the node oneOrSeveralShips does not have the attribute, then it is the single ships configuration.
            if ( !oneOrElement.hasAttribute(QLatin1String("longestShip")) ) {
                return MessagePtr(new GameOptionsMessage(adjacentShips, severalShips, BattleShipsConfiguration::defaultSingleShipsConfiguration(adjacentShips, true)));
            }
            else {
                longestShip = oneOrElement.attribute(QLatin1String("longestShip")).toUInt();
            }
            DEF_ELEMENT(boardWidth);
            DEF_ELEMENT(boardHeight);
            unsigned int width=boardWidth.toUInt();
            unsigned int height=boardHeight.toUInt();
            // and get the ships configuration
            QDomNodeList nodes = main.childNodes();
            BattleShipsConfiguration battleShipsConfiguration(longestShip, adjacentShips, width, height, true);
            for (int i = 0; i < nodes.count(); i++) {
                QDomElement element = nodes.item(i).toElement();
                if (!element.isNull() && element.tagName()==QLatin1String("ships")) {
                    QString name=element.attribute(QLatin1String("name"));
                    QString pluralName=element.attribute(QLatin1String("pluralName"));
                    unsigned int size=element.attribute(QLatin1String("size")).toUInt();
                    unsigned int number=element.attribute(QLatin1String("number")).toUInt();
                    battleShipsConfiguration.addShips(size,number,name,pluralName);
                }
            }
            if ( !battleShipsConfiguration.isAValidConfiguration() )
            {
               return MessagePtr(new GameOptionsMessage(adjacentShips, severalShips, BattleShipsConfiguration::defaultSingleShipsConfiguration(adjacentShips, true)));
            }
            return MessagePtr(new GameOptionsMessage(adjacentShips, severalShips, battleShipsConfiguration));
        }
    default:
        emit parseError("Unknown message type");
        return MessagePtr();
    }
}
#undef DEF_COORD
#undef DEF_ELEMENT

void Protocol::send(const MessagePtr& msg)
{
    m_message_queue.enqueue(msg);
}

void Protocol::sendNext()
{
    if (!m_message_queue.isEmpty())
    {
        MessageSender sender;
        m_message_queue.dequeue()->accept(sender);
        
        QTextStream stream(m_device);
        stream << sender.document().toString() << endl;
        
        qDebug() << "sending:" << sender.document().toString();
    }
}

void Protocol::processDisconnection()
{
    m_timer.stop();
    emit disconnected();
}

