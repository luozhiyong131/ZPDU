#include "zebra_datapacket.h"


Zebra_DataPacket::Zebra_DataPacket()
{
    dhtStatus = false;
}

Zebra_DataPacket *Zebra_DataPacket::bulid()
{
    static Zebra_DataPacket* sington = nullptr;
    if(sington == nullptr)
        sington = new Zebra_DataPacket();

    return sington;
}




sZebraNode *Zebra_DataPacket::find(const QString &key)
{
    sZebraNode * node = nullptr;
    bool ret = contains(key);
    if(ret) {
        auto index = mHash.find(key);
        if(index != mHash.end()) {
            node = index.value();
        }
    }

    return node;
}

bool Zebra_DataPacket::replace(const QString &key, sZebraNode &value)
{
    bool ret = true;
    sZebraNode *node = find(key);
    if(node) {
        node->ip = value.ip;
        node->node_id = value.node_id;
        node->join_id = value.join_id;
        node->channel_id = value.channel_id;
    } else {
        ret = false;
    }
    return ret;
}

bool Zebra_DataPacket::insert(const QString &key, sZebraNode &value)
{
    bool ret = contains(key);
    if(!ret) {
        sZebraNode *node = new sZebraNode;
        mHash.insert(key, node);
    }

    return replace(key, value);
}

QString Zebra_DataPacket::getIp(const QString &id, const QString &ch)
{
    QString ip;
    sZebraNode *node = find(id);
    if(node) {
        if(node->channel_id == ch) {
            ip = node->ip;
            node->status = eZebra_Chat;
        }
    }

    return ip;
}

QStringList Zebra_DataPacket::getChannel(const QString &ip)
{
    QStringList ret;

    QHashIterator<QString, sZebraNode*> iter(mHash);
    while(iter.hasNext())
    {
        iter.next();
        sZebraNode *node = iter.value();

        if(node->ip == ip) {
            ret << node->channel_id;
        }
    }

    return ret;
}

QString Zebra_DataPacket::getIpByNode(const QString &id)
{
    QString ip;
    sZebraNode *node = find(id);
    if(node) {
        ip = node->ip;
    }

    return ip;
}

QString Zebra_DataPacket::getIpByChannel(const QString &id)
{
    QString ret;

    QHashIterator<QString, sZebraNode*> iter(mHash);
    while(iter.hasNext())
    {
        iter.next();
        sZebraNode *node = iter.value();

        if(node->channel_id == id) {
            ret = node->ip;
            if(!ret.isEmpty()) break;
        }
    }

    return ret;
}

sZebraNode *Zebra_DataPacket::findByJoin(const QString &id)
{
    sZebraNode *node = nullptr;
    QHashIterator<QString, sZebraNode*> iter(mHash);
    while(iter.hasNext()) {
        iter.next();
        sZebraNode *it = iter.value();
        if(it->join_id == id) {
            node = it;
            break;
        }
    }

    return node;
}

void Zebra_DataPacket::removeJoin(const QString &id)
{
    sZebraNode *node = findByJoin(id);
    if(node) {
        mHash.remove(node->node_id);
        delete node;
    }
}

bool Zebra_DataPacket::setChannel(const QString &id, const QString &ch)
{
    bool ret = true;
    sZebraNode *node = findByJoin(id);
    if(node) {
        node->status = eZebra_Joined;
        node->channel_id = ch;
    } else {
        ret = false;
    }

    return ret;
}

bool Zebra_DataPacket::setJoinFailed(const QString &id)
{
    bool ret = true;
    sZebraNode *node = findByJoin(id);
    if(node) {
        node->status = eZebra_Disjoin;
    } else {
        ret = false;
    }

    return ret;
}

QString Zebra_DataPacket::getStatus(const QString &ip)
{
    QString ret;
    if(dhtStatus) {
        sZebraNode *node = findByIp(ip);
        if(node) {
            switch (node->status) {
            case eZebra_Joining: ret ="joining"; /*QObject::tr("正在加入分布式网络");*/ break;
            case eZebra_Disjoin: ret = "join faild";/*QObject::tr("加入分布式网络失败");*/ break;
            case eZebra_Joined: ret = "join ok"; /*QObject::tr("加入分布式网络成功"); */break;
            case eZebra_Chat: ret = "chat ok";/*QObject::tr("分布式组网正常");*/ break;
            default: break;
            }
        }
    } else {
        ret = "DHT err"; // QObject::tr("分布式网络异常");
    }


    return ret;
}

sZebraNode *Zebra_DataPacket::findByIp(const QString &ip)
{
    sZebraNode *node = nullptr;
    QHashIterator<QString, sZebraNode*> iter(mHash);
    while(iter.hasNext()) {
        iter.next();
        sZebraNode *it = iter.value();
        if(it->ip == ip)  break;
    }

    return node;
}

bool Zebra_DataPacket::updateData(sZebraNode &node)
{
    bool ret = insert(node.node_id, node);
    if(ret) {
        sZebraNode *it = find(node.node_id);
        if(it->channel_id.isEmpty()) {
            if(it->status < eZebra_Disjoin)
                it->status = eZebra_Joining;
        }
    }

    return ret;
}

QStringList Zebra_DataPacket::getJoinList()
{
    QStringList list;
    QHashIterator<QString, sZebraNode*> iter(mHash);
    while(iter.hasNext()) {
        iter.next();
        sZebraNode *it = iter.value();
        if(it->status < eZebra_Joined)  {
            list << it->join_id;
        }
    }

    return list;
}
