#ifndef ZEBRA_DATAPACKET_H
#define ZEBRA_DATAPACKET_H

#include <QObject>
#include <QtCore>
#include <QDebug>

enum Zebra_Enum
{
    eZebra_Disable,
    eZebra_Joining,
    eZebra_Disjoin,
    eZebra_Joined,
    eZebra_Chat
};

struct sZebraNode
{
    sZebraNode() {status=0;}

    int status;
    QString node_id;
    QString join_id;
    QString channel_id;
    QString ip;
};

class Zebra_DataPacket
{
    Zebra_DataPacket();
public:
    static Zebra_DataPacket *bulid();

    bool dhtStatus; // �ֲ�������
    QString node_id;

    bool updateData(sZebraNode &node);
    QStringList getJoinList();
    void removeJoin(const QString &id);

    sZebraNode *findByJoin(const QString &id);
    bool setJoinFailed(const QString &id);
    bool setChannel(const QString &id, const QString &ch);

    sZebraNode *findByIp(const QString &ip);
    QString getStatus(const QString &ip);
    QStringList getChannel(const QString &ip);
    QString getIpByNode(const QString &id);
    QString getIpByChannel(const QString &id);
    QString getIp(const QString &id, const QString &ch);

    bool replace(const QString &key, sZebraNode &value);
    bool insert(const QString &key, sZebraNode &value);
    bool contains(const QString &key) {return mHash.contains(key);}
    sZebraNode *find(const QString &key);

private:
    QHash<QString, sZebraNode*> mHash;
};

#endif // ZEBRA_DATAPACKET_H
