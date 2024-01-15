#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>
#include <QDataStream>
#include <QObject>

class Client : public QObject
{
    Q_OBJECT
public:
    Client(QString ip, int port);
    void sendToServer(QString str);
    explicit Client(QObject *parent = nullptr);

public slots:
    void slotReadyRead();

signals:
    void getMessage(QString msg);

private:
    QTcpSocket *socket;
    QByteArray Data;
};

#endif // CLIENT_H
