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


void Zebra_Client::key_pair(std::string& public_key, std::string& private_key)
{
	Zebra_sNodeItem *item = mCfg->findNode(mBootstrapNodes);
	if(item) {
		public_key = item->public_key;
		private_key = item->private_key;
	} else {
		cout << "zebra read node key fail." << endl;
	}
}


void Zebra_Client::handleBootstrapEvent(bootstrapEvent *evn)
{
	mBootstrapNodes = evn->node_id();

	Zebra_sNodeItem item;
	item.node_id = evn->node_id();
	item.public_key = evn->key().Publick;
	item.private_key = evn->key().Private;
	mCfg->updateNode(item);
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
		Zebra_sChItem *item = mCfg->getChannel();

		item->channel_id = ch.channel_id();
		item->ch_owner_id = ch.owner().id;
		item->type = ch.type();
		item->expire =ch.expire();

		item->public_key = ch.key().Publick;
		item->private_key = ch.key().Private;
		item->secret = ch.secret();

		item->f_node_id = ch.from().id;
		item->f_pubkey = ch.from().pubkey;
		item->f_privatekey = ch.from().privatekey;

		item->t_node_id = ch.to().id;
		item->t_pubkey = ch.to().pubkey;
		item->t_privatekey = ch.to().privatekey;
		mCfg->saveChannel();
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
	// evn->reason("welcome to here");
	evn->allow(true);
}


void Zebra_Client::handleAllowEvent(allowEvent* evn)
{
	mCfg->join_id = evn->join_channel();
	mCfg->channel_id = evn->chat_channel();
}

void Zebra_Client::handleDisAllowEvent(disAllowEvent* evn)
{
	string join_id = evn->join_channel();
	//    mZebraData->setJoinFailed(join_id);
}

void Zebra_Client::handleMessageEvent(messageEvent* evn)
{
	static char rcv[512] = {0};

	string from = evn->from();
	string id = evn->channel();

	string data =  evn->data();
	int len = data.length();
	memcpy(rcv, data.c_str(), len);

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
	Zebra_sChItem *item = mCfg->getChannel();
	if(item->channel_id.size()) {
		channel::audience_type from;
		from.id = item->f_node_id;
		from.pubkey = item->f_pubkey;
		from.privatekey = item->f_privatekey;

		channel::audience_type to;
		to.id = item->t_node_id;
		to.pubkey = item->t_pubkey;
		to.privatekey = item->t_privatekey;

		channel::Keypair key;
		key.Private = item->private_key;
		key.Publick = item->public_key;

		channel::Owner own;
		own.id = item->ch_owner_id;
		own.keys = key;

		channel ch(item->type, item->expire, item->channel_id, key, own, item->secret, from, to);
		evn->add(ch);
	}
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
