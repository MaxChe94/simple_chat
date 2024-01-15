#include "client.h"

Client::Client(QString ip, int port)
{
   socket = new QTcpSocket(this);
   socket->connectToHost(ip, port);
}

void Client::sendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out <<str;
    socket->write(Data);
}

void Client::slotReadyRead()
{
    QDataStream in(socket);
    if (in.status() == QDataStream::Ok)
    {
        QString str;
        in >> str;
        emit Client::getMessage(str);
    }
    else
    {
        emit Client::getMessage("ошибка чтения...");
    }
}


