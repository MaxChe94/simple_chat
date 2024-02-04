#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>
#include <QDataStream>
#include <QObject>

class Client : public QObject
{
    Q_OBJECT
public:
    Client(QString ip, int port, QString name);
    ~Client();
    void sendToServer(QString str);
    void openConnection();
    explicit Client(QObject *parent = nullptr);

public slots:
    void slotReadyRead();
signals:
    void getMessage(QString msg);
    void updateConnectState(int state, QString text = "");

private:
    QTcpSocket *socket;
    QByteArray Data;
    quint16 blockSize = 0;
    QString ip;
    int port;
    QString name;
    QString serverName;
    bool serverNameReceived = false;
    bool close = false;
};

#endif // CLIENT_H
