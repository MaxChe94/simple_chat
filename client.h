#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>

#include <QObject>

class Client : public QTcpSocket
{
    Q_OBJECT
public:
    Client(QString ip, int port);
    explicit Client(QObject *parent = nullptr);

public slots:
    void slotReadyRead();

signals:
    void getMessage(QString msg);
};

#endif // CLIENT_H
