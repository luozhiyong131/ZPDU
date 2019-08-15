/*
 *
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "zebra_client.h"

#define ZEBRA_FN "zebracfg.ini"

#define zebra_tostd(str)    str.toStdString(). c_str()
#define zebra_fromstd(str)  QString::fromStdString(str.c_str())


#include "zebra_peer.h"

Zebra_Client::Zebra_Client()
{
    mZebraData = Zebra_DataPacket::bulid();
}


void Zebra_Client::handleBootstrapEvent(bootstrapEvent *evn)
{
    mNodeId = evn->node_id();
}


void Zebra_Client::handleGetDHTNodesEvent(dhtNodeEvent *evn) {
    std::vector<std::string> nodes;
    evn->nodes(nodes);
}


void Zebra_Client::handleCreateChannelEvent(channelEvent* evn)
{
    channel ch =  evn->getChannel();
    if(channelType::CH_CHAT == ch.type()) {
//        qDebug() << "zebra create channel " << ch.channel_id().c_str();
    }
}


void Zebra_Client::handleConnectPeerEvent(connectEvent* evn)
{
    Zebra_DataPacket *dp = Zebra_DataPacket::bulid();
    if (evn->error_code()) {
        dp->dhtStatus = false;
        qDebug() << "DHT connected err";
    } else {
        dp->dhtStatus = true;
//        qDebug() << "DHT connected ok";
    }
}


void Zebra_Client::handleJoinChannelEvent(joinEvent* evn)
{
    evn->reason("welcome to here");
    evn->allow(true);
}


void Zebra_Client::handleAllowEvent(allowEvent* evn)
{
    QString join_id = zebra_fromstd(evn->join_channel());
    QString channel_id = zebra_fromstd(evn->chat_channel());
    mZebraData->setChannel(join_id, channel_id);
}

void Zebra_Client::handleDisAllowEvent(disAllowEvent* evn)
{
    QString join_id = zebra_fromstd(evn->join_channel());
    mZebraData->setJoinFailed(join_id);
}

void Zebra_Client::handleMessageEvent(messageEvent* evn)
{
    static char rcv[512] = {0};

    QString from = zebra_fromstd(evn->from());
    QString id = zebra_fromstd(evn->channel());

    std::string data =  evn->data();
    int len = data.length();
    memcpy(rcv, data.c_str(), len);
    QByteArray ba(rcv, len);    
    QString ip = mZebraData->getIp(from, id);

    qDebug() << ip << ba;

    Zebra_Peer *peer = Zebra_Peer::bulid();
    peer->send(ip, (uchar *)rcv, len);
}


void Zebra_Client::handleReplyEvent(replyEvent* evn)
{
    qDebug() << "Zebra A reply "
             << "  errorCode: " << evn->errorCode()
             << "  originator: " << evn->originatorType()
             << "  channel: " << zebra_fromstd(evn->channel()) ;
}



void Zebra_Client::handleNotifyEvent(notifyEvent* evn)
{
    qDebug() << "Zebra A notify \n"
             << "  channel: " << zebra_fromstd(evn->channel())
             << "  reason: " << evn->reason();
}


void Zebra_Client::handleLoadChEvent(localChannel* evn)
{
}


void Zebra_Client::onEvent(eventCode code, event* evn)
{
    switch (code) {
    case BOOTSTRAP: // 引导完成通知事件
        handleBootstrapEvent(dynamic_cast<bootstrapEvent*>(evn));
        break;

    case GETDHTNODES: //路由表打印事件
        handleGetDHTNodesEvent(dynamic_cast<dhtNodeEvent*>(evn));
        break;

    case CREATECHANNEL: // 创建通道通知事件
        handleCreateChannelEvent(dynamic_cast<channelEvent*>(evn));
        break;

    case CONNECTION: // 连接事件
        handleConnectPeerEvent(dynamic_cast<connectEvent*>(evn));
        break;

    case JOINCHANNEL: // join请求事件
        handleJoinChannelEvent(dynamic_cast<joinEvent*>(evn));
        break;

    case ALLOWCHANNEL: // 同意join事件
        handleAllowEvent(dynamic_cast<allowEvent*>(evn));
        break;

    case  DISALLOWCHANNEL: // 不同意join事件
        handleDisAllowEvent(dynamic_cast<disAllowEvent*>(evn));
        break;

    case CHAT: // 消息通知事件
        handleMessageEvent(dynamic_cast<messageEvent*>(evn));
        break;

    case REPLY:  // 用户请求应答事件
        handleReplyEvent(dynamic_cast<replyEvent*>(evn));
        break;

    case NOTIFY: // 其他通知事件
        handleNotifyEvent(dynamic_cast<notifyEvent*>(evn));
        break;

    case LOADLOCALCHANNEL:
        handleLoadChEvent(dynamic_cast<localChannel*>(evn));
        break;

    default:
        break;
    }

}
