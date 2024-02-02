#include "connection.h"

Connection::Connection(bool is_server)
{
    server_mode = is_server;
}

Connection::~Connection()
{
    if (server_mode){
        delete this->server;
    } else {
        delete this->client;
    }
    qDebug() << "Соединение закрыто";
}

void Connection::start(QString name, QString ip, int port)
{
    if (server_mode){
        server = new Server(port, name);
        connect(server, &Server::getMessage, this, &Connection::messageReceived);
        connect(server, &Server::updateConnectState, this, &Connection::getConnectState);
        server->startListen();
    } else {
        client = new Client(ip, port, name);
        connect(client, &Client::getMessage, this, &Connection::messageReceived);
        connect(client, &Client::updateConnectState, this, &Connection::getConnectState);
        client->openConnection();
    }
    //name = name;
}

void Connection::getConnectState(int state, QString text)
{
    emit this->gettingNewState(state, text);
}

void Connection::sendMsg(QString Data)
{
    if (server_mode){
        server->SendToClient(Data);
    } else {
        client->sendToServer(Data);
    }
}

