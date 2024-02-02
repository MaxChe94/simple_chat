#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

struct ClientInfo {
    qintptr socketDescriptor;
    QString username;
    QTcpSocket *socket;
};


class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(int port, QString name);
    ~Server();
    void SendToClient(QString data);
    void startListen();

private:
    QVector <quintptr> pendingSockets;
    QByteArray Data;
    quint16 blockSize = 0;
    QTcpSocket *socket;
    int port;
    QString name;
    QVector<ClientInfo> clients;

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
    void clientDisconnected();

signals:
    void getMessage(QString msg);
    void updateConnectState(int state, QString text = "");
};

#endif // SERVER_H
