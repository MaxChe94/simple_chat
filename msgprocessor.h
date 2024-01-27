#ifndef MSGPROCESSOR_H
#define MSGPROCESSOR_H

#include <QObject>

enum mtype {
        Getname,

        Data,
        File
    };
Q_ENUM(mtype);

stuct Msg{
    quint16 id;
    mtype type;
    quint16 part;
    quint16 msize;
    QByteArray data;
};

class MsgProcessor : public QObject
{
    Q_OBJECT
public:
    explicit MsgProcessor(QObject *parent = nullptr);
    MsgProcessor(QString name);
    void receiveData(QBitArray);

signals:

private:
    QString name;
    QByteArray buf;
};

#endif // MSGPROCESSOR_H
