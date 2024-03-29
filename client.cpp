#include "client.h"

Client::Client(QString ip, int port, QString name)
{
   socket = new QTcpSocket(this);
   this->ip = ip;
    this->port = port;
   this->name = name;
}

Client::~Client()
{
    close = true;
    socket->close();
    qDebug() << "Client closed";
}

void Client::sendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out << quint16(0) << str;
    out.device()->seek(0);
    out << quint16(str.size() - sizeof (quint16));
    socket->write(Data);
}

void Client::openConnection()
{
    connect(socket, &QTcpSocket::stateChanged, this, [this](QAbstractSocket::SocketState state) {
        if (state == QAbstractSocket::UnconnectedState) {
            if (!this->close){
                emit updateConnectState(4, serverName);
                // Дополнительные действия при отключении сервера
            }
        }
    });
    connect(socket, &QTcpSocket::connected, this, [this](){
        sendToServer(name);
    });
    connect(socket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    socket->connectToHost(ip, port);

}

void Client::slotReadyRead()
{
    QDataStream in(socket);
    if (in.status() == QDataStream::Ok)
    {
        QString str;
            if (!serverNameReceived) {
                // Это первое сообщение после подключения, которое содержит имя сервера
                in >> str;
                serverName = str;
                serverNameReceived = true;
                qDebug() << "Connected to server: " << serverName;
                emit updateConnectState(3, serverName);
            } else {
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
                    in >> str;
                    emit Client::getMessage(str);
                    blockSize = 0;
            }
        }
    }
    else
    {
        emit Client::getMessage("ошибка чтения...");
    }
}
