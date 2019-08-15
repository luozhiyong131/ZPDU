/*
 *
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "zebra_peer.h"


Zebra_Peer::Zebra_Peer(QObject *parent) : QObject(parent)
{
    mClient = new Zebra_Client;
    mClient->setBootstrapNodes("192.168.1.207:37053");

    mIm = Zebra_Im::bulid(mClient);
    mIm->startZebra();

    mUdp = new Zebra_UdpSocket(this);
    mJoin = new Zebra_JoinThread(this);
}

Zebra_Peer *Zebra_Peer::bulid(QObject *parent)
{
    static Zebra_Peer* sington = nullptr;
    if(sington == nullptr)
        sington = new Zebra_Peer(parent);

    return sington;
}

int Zebra_Peer::send(const QString &ip, uchar *buf, int len)
{
    int ret = 0;
    QStringList ids  = Zebra_DataPacket::bulid()->getChannel(ip);
    for(int i=0; i<ids.size(); ++i) {
        ret = mIm->send(ids.at(i), buf, len);;
    }

    return ret;
}

QString Zebra_Peer::getStatus(const QString &ip)
{
    return Zebra_DataPacket::bulid()->getStatus(ip);
}

QString Zebra_Peer::getDhtStatus()
{
    QString ret =  "DHT Err" ;//tr("分布式网络异常");
    bool status = Zebra_DataPacket::bulid()->dhtStatus;
    if(status) {
        ret = "DHT OK"; // tr("分布式网络正常");
    }

    return ret;
}
