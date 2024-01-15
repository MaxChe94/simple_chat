#include "chatmessage.h"
#include <QDataStream>

ChatMessage::ChatMessage(QObject *parent)
    : QObject(parent)
{
}

ChatMessage::ChatMessage(const QString &username, const QString &message, QObject *parent)
    : QObject(parent), username(username), message(message)
{
}

QList<QByteArray> ChatMessage::serialize() const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << username << message;

    const int maxPacketSize = 1092;
    int totalSize = data.size();
    int currentPos = 0;
    int remainingSize = totalSize;

    QList<QByteArray> packets;

    int currentPart = 0;

    while (remainingSize > 0) {
        QByteArray packet;
        QDataStream packetStream(&packet, QIODevice::WriteOnly);

        int packetSize = qMin(remainingSize, maxPacketSize);
        packetStream << totalSize << currentPart << totalParts << data.mid(currentPos, packetSize);

        packets.append(packet);

        currentPos += packetSize;
        remainingSize -= packetSize;

        // Увеличиваем currentPart только при формировании последнего пакета
        if (remainingSize == 0) {
            currentPart++;
        }
    }

    return packets;
}

ChatMessage ChatMessage::deserialize(const QList<QByteArray> &packets)
{
    ChatMessage msg;

    for (const QByteArray &packet : packets) {
        QDataStream stream(packet);

        qint32 packetTotalSize;
        qint32 packetCurrentPart;
        qint32 packetTotalParts;
        QByteArray packetData;

        stream >> packetTotalSize >> packetCurrentPart >> packetTotalParts >> packetData;

        msg.totalSize = packetTotalSize;
        msg.currentPart = packetCurrentPart;
        msg.totalParts = packetTotalParts;

        msg.data.append(packetData);
    }

    if (msg.currentPart == msg.totalParts && msg.data.size() == msg.totalSize) {
        QDataStream dataStream(msg.data);
        dataStream >> msg.username >> msg.message;

        emit msg.messageReceived(msg.username, msg.message);
    }

}

QString ChatMessage::getUsername() const
{
    return username;
}

void ChatMessage::setUsername(const QString &value)
{
    username = value;
}

void ChatMessage::setMessage(const QString &value)
{
    message = value;
}
