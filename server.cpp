#include "server.h"




Server::Server(int port)
{
    this->listen(QHostAddress::LocalHost, port);
}

void Server::SendToClient(QString data)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out << quint16(0) << data;
    out.device()->seek(0);
    out << quint16(data.size() - sizeof (quint16));
    for (int i = 0; i < Sockets.size(); i++){
        Sockets[i]->write(Data);
    }
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
        for (;;){
            if (blockSize == 0){
                if (socket->bytesAvailable() < 2){
                    break;
                }
                in >> blockSize;
            }
            if (socket->bytesAvailable() < blockSize){
                break;
            }
            in >> data;
            blockSize = 0;
            emit getMessage(data);
        }
    }
    else
    {
        emit getMessage("DataStream error");
    }
}

void Server::clientDisconnected()
{
    QTcpSocket *disconnectedSocket = qobject_cast<QTcpSocket*>(sender());
    if (disconnectedSocket) {
        Sockets.removeOne(disconnectedSocket);
        disconnectedSocket->deleteLater();
        qDebug() << "Клиент отключился";
    }
}
