#ifndef ZEBRA_UDPTHREAD_H
#define ZEBRA_UDPTHREAD_H

#include <QUdpSocket>
#include "zebra_jointhread.h"

class Zebra_UdpSocket : public QThread
{
    Q_OBJECT
public:
    explicit Zebra_UdpSocket(QObject *parent = nullptr);

protected:
    void workDown(const QString &ip, QString &msg);
    bool sentBroadcastData(const QByteArray &msg, quint16 port);

public slots:
    void receiveSlot();
    void sentSlot();

private:
    QUdpSocket *mSocket;
};

#endif // ZEBRA_UDPTHREAD_H
