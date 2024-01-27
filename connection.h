#ifndef CONNECTION_H
#define CONNECTION_H
#include <server.h>
#include <client.h>
#include <QByteArray>

class Connection : public QObject
{
    Q_OBJECT
public:
    Connection(bool is_server);
    ~Connection();
    void sendMsg(QString Data);
    void start(QString name = "", QString ip = "", int port = 0);

private:
    bool server_mode;
    Server* server;
    Client* client;
    QString name;

signals:
    void messageReceived(QString Data);
};



#endif // CONNECTION_H
