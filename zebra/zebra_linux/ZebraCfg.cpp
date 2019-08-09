/*
 * ZebraCfg.cpp
 *
 *  Created on: 2019年8月9日
 *      Author: Lzy
 */

#include "ZebraCfg.h"

Zebra_Cfg::Zebra_Cfg()
{
	dhtStatus = false;
	mNode = new Zebra_sNodeItem;
	mChannel = new Zebra_sChItem;
}

Zebra_Cfg::~Zebra_Cfg()
{
}

Zebra_Cfg *Zebra_Cfg::bulid()
{
	static Zebra_Cfg* sington = new Zebra_Cfg();;
	return sington;
}

bool Zebra_Cfg::saveChannel()
{
	channel_id = mChannel->channel_id;

	return true;
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
}
