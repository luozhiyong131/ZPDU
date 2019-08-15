/*
 * ZebraPeer.cpp
 *
 *  Created on: 2019年8月12日
 *      Author: Lzy
 */

#include "ZebraPeer.h"

Zebra_Peer::Zebra_Peer()
{
	zebra_udp_thread();
	mIm = Zebra_Im::bulid();
	mIm->startZebra();
}

Zebra_Peer::~Zebra_Peer()
{
}

Zebra_Peer *Zebra_Peer::bulid()
{
	static Zebra_Peer* sington = nullptr;
	if(sington == nullptr)
		sington = new Zebra_Peer();

	return sington;
}


int Zebra_Peer::send(uchar *buf, int len)
{
	int ret = 0;
	Zebra_Cfg *cfg = Zebra_Cfg::bulid();
	if(cfg->dhtStatus) {
		ret =  mIm->send(cfg->channel_id, buf, len);
	}

	return ret;
}
