#ifndef ZEBRA_PEER_H
#define ZEBRA_PEER_H
#include "zebra_udpsocket.h"

class Zebra_Peer : public QObject
{
    Q_OBJECT
    explicit Zebra_Peer(QObject *parent = nullptr);
public:
    static Zebra_Peer *bulid(QObject *parent = nullptr);

    QString getDhtStatus();
    QString getStatus(const QString &ip);
    int send(const QString &ip, uchar *buf, int len);

private:
    Zebra_Im *mIm;
    Zebra_Client *mClient;
    Zebra_UdpSocket *mUdp;
    Zebra_JoinThread *mJoin;
};

#endif // ZEBRA_PEER_H
