/*
 * ZebraChannel.cpp
 *
 *  Created on: 2019年8月12日
 *      Author: Lzy
 */

#include "ZebraChannel.h"

Zebra_Channel::Zebra_Channel()
{

}

Zebra_Channel::~Zebra_Channel()
{

}


bool Zebra_Channel::checkChannel()
{
	bool ret = false;
	Zebra_Cfg *cfg = Zebra_Cfg::bulid();
//	if(cfg->dhtStatus)
	{
		if(cfg->join_id.size()<2)
			ret = true;
	}
	return ret;
}


void Zebra_Channel::createChannel()
{
	string channel_id;
	Zebra_Im *im = Zebra_Im::bulid();
	Zebra_Cfg *cfg = Zebra_Cfg::bulid();

	if(checkChannel()) {
		int ret = im->createChannel(0, 0, "zpdu", channel_id);
		if(ret < 0) {
			cout << ret << "create channel err id:" << channel_id.c_str() << endl;
		} else {
			cfg->join_id = channel_id.c_str();
//			cout << ret << "create channel id:" << channel_id.c_str() << endl;
		}
	}

}
