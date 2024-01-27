#include "connection.h"

Connection::Connection(bool is_server)
{
    server_mode = is_server;
}

Connection::~Connection()
{
    if (server_mode){
        delete server;
    } else {
        delete client;
    }
    this->name = name;
}

void Connection::start(QString name, QString ip, int port)
{
    if (server_mode){
        server = new Server(port);
        connect(server, &Server::getMessage, this, &Connection::messageReceived);
    } else {
        client = new Client(ip, port);
        connect(client, &Client::getMessage, this, &Connection::messageReceived);
    }
    //name = name;
}

void Connection::sendMsg(QString Data)
{
    if (server_mode){
        server->SendToClient(Data);
    } else {
        client->sendToServer(Data);
    }
}

