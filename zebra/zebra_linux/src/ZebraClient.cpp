/*
 * ZebraClient.cpp
 *
 *  Created on: 2019年8月9日
 *      Author: Lzy
 */

#include "ZebraClient.h"

Zebra_Client::Zebra_Client()
{
	mCfg = Zebra_Cfg::bulid();
}

Zebra_Client::~Zebra_Client()
{
}


void Zebra_Client::handleBootstrapEvent(bootstrapEvent *evn)
{
	mNodeId =  evn->node_id().c_str();
	if(mNodeId.size()) {
		mCfg->node_id = mNodeId;
	}
//	cout << "boot node : " << mNodeId.c_str() << endl;
}


void Zebra_Client::handleGetDHTNodesEvent(dhtNodeEvent *evn) {
	std::vector<std::string> nodes;
	evn->nodes(nodes);
}


void Zebra_Client::handleCreateChannelEvent(channelEvent* evn)
{
	channel ch =  evn->getChannel();
	if(CH_CHAT == ch.type())
	{
		mCfg->channel_id = ch.channel_id().c_str();
		cout << "zebra Create Channel id:  " << mCfg->channel_id << endl;
	}
}


void Zebra_Client::handleConnectPeerEvent(connectEvent* evn)
{
	if (evn->error_code()) {
		mCfg->dhtStatus = false;
		cout << "DHT connected err" << endl;
	} else {
		mCfg->dhtStatus = true;
		cout << "DHT connected ok" << endl;
	}
}


void Zebra_Client::handleJoinChannelEvent(joinEvent* evn)
{
	evn->reason("welcome to here");
	evn->allow(true);
	cout << "join allow " << evn->channel().c_str() << endl;
}


void Zebra_Client::handleAllowEvent(allowEvent* evn)
{
	mCfg->join_id = evn->join_channel();
	mCfg->channel_id = evn->chat_channel();
}

void Zebra_Client::handleDisAllowEvent(disAllowEvent* evn)
{
	string join_id = evn->join_channel();
	cout << "join dis allow " << join_id << endl;
}

void Zebra_Client::handleMessageEvent(messageEvent* evn)
{
	static char rcv[512] = {0};

	string from = evn->from();
	string id = evn->channel();

	string data =  evn->data();
	int len = data.length();
	memcpy(rcv, data.c_str(), len);

	cout <<"recv: " << rcv << endl;
}


void Zebra_Client::handleReplyEvent(replyEvent* evn)
{
	cout << "Zebra A reply "
			<< "  errorCode: " << evn->errorCode()
			<< "  originator: " << evn->originatorType()
			<< "  channel: " << evn->channel() << endl;
}



void Zebra_Client::handleNotifyEvent(notifyEvent* evn)
{
	cout << "Zebra A notify \n"
			<< "  channel: " << evn->channel()
			<< "  reason: " << evn->reason() << endl;
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
