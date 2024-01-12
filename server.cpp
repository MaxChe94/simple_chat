#include "server.h"


//Server::Server():
//{

//}

void Server::SendToClient(QString data)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out << data;
    socket->write(Data);
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    Sockets.push_back(socket);
    qDebug() << "client connected";
}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    if (in.status() == QDataStream::Ok)
    {
        qDebug() << "read...";
        QString data;
        in >> data;
        qDebug() << data;
    }
    else
    {
        qDebug() << "DataStream error";
    }
}
