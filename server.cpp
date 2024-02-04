#include "server.h"

Server::Server(int port, QString name)
{
    this->port = port;
    this->name = name;
}

Server::~Server()
{
    for (int i = 0; i < this->clients.size(); i++)
    {
        clients[i].socket->close();
    }
    this->close();
    qDebug() << "Server closed";
}

void Server::SendToClient(QString data)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out << quint16(0) << data;
    out.device()->seek(0);
    out << quint16(data.size() - sizeof (quint16));
    for (int i = 0; i < clients.size(); i++){
        clients[i].socket->write(Data);
    }
}

void Server::startListen()
{
    if (this->listen(QHostAddress::LocalHost, port))
    {
        emit this->updateConnectState(0);
        qDebug() << "Server started";
    }
    else
    {
        delete this;
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);
    QDataStream out(&Data, QIODevice::WriteOnly);
    out << name;
    socket->write(Data);
//    // Создаем объект ClientInfo и добавляем его в список clients
//    ClientInfo client;
//    client.socketDescriptor = socketDescriptor;
//    client.username = username;
//    clients.push_back(client);

    //qDebug() << "Client connected: " << username;
    //emit updateConnectState(1, username);
    pendingSockets.push_back(socketDescriptor);
}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QString data;
    QDataStream in(socket);
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
        if (pendingSockets.contains(socket->socketDescriptor())) {
            //QByteArray usernameData = socket->readAll().trimmed();
            //in >> data;
            ClientInfo client;
            client.socketDescriptor = socket->socketDescriptor();
            client.username = data;
            client.socket = socket;
            clients.push_back(client);
                    //qintptr ds = socket->socketDescriptor();
            pendingSockets.removeOne(socket->socketDescriptor());
            emit updateConnectState(1, data);
            }
            else {
                emit getMessage(data);
            }
        }
    }

void Server::clientDisconnected()
{
    QTcpSocket *disconnectedSocket = qobject_cast<QTcpSocket*>(sender());
    if (!disconnectedSocket) {
        return;
    }

    // Находим и удаляем соответствующий элемент из списка clients
    //qintptr disconDescriptor = disconnectedSocket->socketDescriptor();
        for (int i = 0; i < clients.size(); ++i) {
        //if (clients[i].socketDescriptor == disconDescriptor) {
        if (clients[i].socket == disconnectedSocket) {
            updateConnectState(2, clients[i].username);
            clients.remove(i);
            break;
        }
    }
    if (clients.size() == 0){
        emit this->updateConnectState(0);
    }
}
