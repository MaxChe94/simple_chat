#include "client.h"

Client::Client(QString ip, int port)
{
    this->connectToHost(ip, port);
}

