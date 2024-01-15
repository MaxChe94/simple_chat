#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QList>

class ChatMessage : public QObject
{
    Q_OBJECT

public:
    ChatMessage(QObject *parent = nullptr);
    ChatMessage(const QString &username, const QString &message, QObject *parent = nullptr);

    QList<QByteArray> serialize() const;
    static ChatMessage deserialize(const QList<QByteArray> &packets);

    // Геттеры и сеттеры (при необходимости)
    QString getUsername() const;
    void setUsername(const QString &value);

    void getMessage() const;
    void setMessage(const QString &value);

signals:
    void messageReceived(const QString &username, const QString &message);

private:
    QString username;
    QString message;
    qint32 totalSize = 0;
    qint32 currentPart = 0;
    qint32 totalParts = 0;
    QByteArray data;
};

#endif // CHATMESSAGE_H
