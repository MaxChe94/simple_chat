#include "client.h"

Client::Client(QString ip, int port)
{
   socket = new QTcpSocket(this);
   /*connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
              this, SLOT(socketError(QAbstractSocket::SocketError)));*/
   socket->connectToHost(ip, port);
   connect(socket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
   connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
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

void Client::slotReadyRead()
{
    QDataStream in(socket);
    if (in.status() == QDataStream::Ok)
    {
        QString str;
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
            blockSize = 0;
            emit Client::getMessage(str);
        }
    }
    else
    {
        emit Client::getMessage("ошибка чтения...");
    }
}

void Client::socketError(QAbstractSocket::SocketError error)
{
    if (error == QAbstractSocket::RemoteHostClosedError) {
        // Сервер отключился
        qDebug() << "Сервер отключился";
        // Можно предпринять какие-то действия, например, попытаться переподключиться
    } else {
        // Обработка других ошибок
        qDebug() << "Произошла ошибка: " << socket->errorString();
    }
}


