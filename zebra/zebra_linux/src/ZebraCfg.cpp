/*
 * ZebraCfg.cpp
 *
 *  Created on: 2019年8月9日
 *      Author: Lzy
 */

#include "ZebraCfg.h"

#define ZEBRA_FN "zebracfg.ini"


Zebra_Cfg::Zebra_Cfg()
{
	dhtStatus = false;
	mNode = new Zebra_sNodeItem;
	mChannel = new Zebra_sChItem;

	readNode();
	readChannel();

	node_id = mNode->node_id;
	channel_id = mChannel->channel_id;
}

Zebra_Cfg::~Zebra_Cfg()
{
}

Zebra_Cfg *Zebra_Cfg::bulid()
{
	static Zebra_Cfg* sington = new Zebra_Cfg();;
	return sington;
}

void Zebra_Cfg::saveNode()
{
	cfg_item_write(ZEBRA_FN, "join_id", join_id.c_str());
	cfg_item_write(ZEBRA_FN, "node_id", mNode->node_id.c_str());
	cfg_item_write(ZEBRA_FN, "public_key", mNode->public_key.c_str());
	cfg_item_write(ZEBRA_FN, "private_key", mNode->private_key.c_str());
}

void Zebra_Cfg::readNode()
{
	char buf[32] = {0}; int len = 0;
	memset(buf, 0, sizeof(buf));

	cfg_item_read(ZEBRA_FN, "join_id", buf,  &len);
	join_id = buf; memset(buf, 0, sizeof(buf));

	cfg_item_read(ZEBRA_FN, "node_id", buf,  &len);
	mNode->node_id = buf; memset(buf, 0, sizeof(buf));

	cfg_item_read(ZEBRA_FN, "public_key", buf,  &len);
	mNode->public_key = buf; memset(buf, 0, sizeof(buf));

	cfg_item_read(ZEBRA_FN, "private_key", buf,  &len);
	mNode->private_key = buf; memset(buf, 0, sizeof(buf));
}

void Zebra_Cfg::saveChannel()
{
	char buf[10] = {0};
	channel_id = mChannel->channel_id;

	sprintf(buf, "%d", mChannel->expire);
	cfg_item_write(ZEBRA_FN, "ch_expire", buf);

	sprintf(buf, "%d", mChannel->type);
	cfg_item_write(ZEBRA_FN, "ch_type", buf);

	cfg_item_write(ZEBRA_FN, "channel_id", mChannel->channel_id.c_str());
	cfg_item_write(ZEBRA_FN, "ch_owner_id", mChannel->ch_owner_id.c_str());
	cfg_item_write(ZEBRA_FN, "ch_public_key", mChannel->public_key.c_str());
	cfg_item_write(ZEBRA_FN, "ch_private_key", mChannel->private_key.c_str());
	cfg_item_write(ZEBRA_FN, "secret", mChannel->secret.c_str());

	cfg_item_write(ZEBRA_FN, "f_node_id", mChannel->f_node_id.c_str());
	cfg_item_write(ZEBRA_FN, "f_pubkey", mChannel->f_pubkey.c_str());
	cfg_item_write(ZEBRA_FN, "f_privatekey", mChannel->f_privatekey.c_str());

	cfg_item_write(ZEBRA_FN, "t_node_id", mChannel->t_node_id.c_str());
	cfg_item_write(ZEBRA_FN, "t_pubkey", mChannel->t_pubkey.c_str());
	cfg_item_write(ZEBRA_FN, "t_privatekey", mChannel->t_privatekey.c_str());
}



void Zebra_Cfg::readChannel()
{
	char buf[32] = {0}; int len = 0;
	memset(buf, 0, sizeof(buf));

	cfg_item_read(ZEBRA_FN, "ch_expire", buf,  &len);
	mChannel->expire = atoi(buf); memset(buf, 0, sizeof(buf));
	cfg_item_read(ZEBRA_FN, "ch_type", buf,  &len);
	mChannel->type = atoi(buf); memset(buf, 0, sizeof(buf));

	cfg_item_read(ZEBRA_FN, "channel_id", buf,  &len);
	mChannel->channel_id = buf; memset(buf, 0, sizeof(buf));
	cfg_item_read(ZEBRA_FN, "ch_owner_id", buf,  &len);
	mChannel->ch_owner_id = buf; memset(buf, 0, sizeof(buf));
	cfg_item_read(ZEBRA_FN, "ch_public_key", buf,  &len);
	mChannel->public_key = buf; memset(buf, 0, sizeof(buf));
	cfg_item_read(ZEBRA_FN, "ch_private_key", buf,  &len);
	mChannel->private_key = buf; memset(buf, 0, sizeof(buf));
	cfg_item_read(ZEBRA_FN, "secret", buf,  &len);
	mChannel->secret = buf; memset(buf, 0, sizeof(buf));

	cfg_item_read(ZEBRA_FN, "f_node_id", buf,  &len);
	mChannel->f_node_id = buf; memset(buf, 0, sizeof(buf));
	cfg_item_read(ZEBRA_FN, "f_pubkey", buf,  &len);
	mChannel->f_pubkey = buf; memset(buf, 0, sizeof(buf));
	cfg_item_read(ZEBRA_FN, "f_privatekey", buf,  &len);
	mChannel->f_privatekey = buf; memset(buf, 0, sizeof(buf));

	cfg_item_read(ZEBRA_FN, "t_node_id", buf,  &len);
	mChannel->t_node_id = buf; memset(buf, 0, sizeof(buf));
	cfg_item_read(ZEBRA_FN, "t_pubkey", buf,  &len);
	mChannel->t_pubkey = buf; memset(buf, 0, sizeof(buf));
	cfg_item_read(ZEBRA_FN, "t_privatekey", buf,  &len);
	mChannel->t_privatekey = buf; memset(buf, 0, sizeof(buf));
}



Zebra_sChItem *Zebra_Cfg::getChannel()
{
	channel_id = mChannel->channel_id;
	return mChannel;
}

Zebra_sNodeItem *Zebra_Cfg::findNode(string &id)
{
	Zebra_sNodeItem *node = NULL;
	if(mNode->node_id == id) {
		node = mNode;
		node_id = node->node_id;
	} else {
		cout << "!!! zebra cfg findNode err!!!" <<id << endl;
	}

	return node;
}

void Zebra_Cfg::updateNode(Zebra_sNodeItem &node)
{
	node_id = node.node_id;
	mNode->node_id = node.node_id;
	mNode->public_key = node.public_key;
	mNode->private_key = node.private_key;
	saveNode();
}
