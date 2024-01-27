#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>


class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(int port);
    void SendToClient(QString data);

private:
    QVector <QTcpSocket*> Sockets;
    QByteArray Data;
    quint16 blockSize = 0;
    QTcpSocket *socket;

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
    void clientDisconnected();

signals:
    void getMessage(QString msg);
};

#endif // SERVER_H
