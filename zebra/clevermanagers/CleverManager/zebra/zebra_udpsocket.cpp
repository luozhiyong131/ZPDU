#include "zebra_udpsocket.h"

const quint16 ZEBRA_UDP_PORT = 27071;

Zebra_UdpSocket::Zebra_UdpSocket(QObject *parent) : QThread(parent)
{
    mSocket = new QUdpSocket(this);
    mSocket->bind(ZEBRA_UDP_PORT);
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(receiveSlot()));
    QTimer::singleShot(1500,this,SLOT(sentSlot()));
}

void Zebra_UdpSocket::sentSlot()
{
    QString str = "zebra manger start";
    quint16 port = ZEBRA_UDP_PORT +1;
    sentBroadcastData(str.toLatin1(), port);
}

bool Zebra_UdpSocket::sentBroadcastData(const QByteArray &msg, quint16 port)
{
    bool ret = false;

    int len = mSocket->writeDatagram(msg.data(),msg.size(),QHostAddress::Broadcast,port);
    if(len != msg.length()) {
        qDebug() << "UdpServer SentData Err" << msg;
    } else {
         ret = true;
    }

    return ret;
}

void Zebra_UdpSocket::workDown(const QString &ip, QString &msg)
{
    sZebraNode node;
    node.ip = ip;

    QStringList list = msg.split("; ");
    for(int i=0; i<list.size(); ++i) {
        QString str = list.at(i);
        if(str.contains("node_id")) {
            node.node_id = str.remove("node_id = ");
        }
        if(str.contains("join_id")) {
            node.join_id = str.remove("join_id = ");
        }
        if(str.contains("channel_id")) {
            node.channel_id = str.remove("channel_id = ");
        }
    }
    Zebra_DataPacket::bulid()->updateData(node);
}

void Zebra_UdpSocket::receiveSlot()
{
    QByteArray ba;
    while(mSocket->hasPendingDatagrams())
    {
        QHostAddress addr; //对方的ip
        ba.resize(mSocket->pendingDatagramSize());
        mSocket->readDatagram(ba.data(), ba.size(), &addr);

        QString data(ba);
        QString ip = addr.toString().remove("::ffff:");
        workDown(ip, data);
    }
}
