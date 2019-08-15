/*
 * ZebraIm.cpp
 *
 *  Created on: 2019年8月9日
 *      Author: Lzy
 */

#include "ZebraIm.h"


Zebra_Im::Zebra_Im(Zebra_Client *c) : im(c)
{
	isRun = false;
}

Zebra_Im::~Zebra_Im()
{
	stopZebra();
}

Zebra_Im *Zebra_Im::bulid()
{
	static Zebra_Im* sington = NULL;
	static Zebra_Client *c = NULL;
	if(sington == NULL) {
		c = new Zebra_Client;
		c->setBootstrapNodes("192.168.1.207:37053");
		sington = new Zebra_Im(c);
	}

	return sington;
}

bool Zebra_Im::startZebra()
{
	if(!isRun) {
		start();
		isRun = true;
	}

	return isRun;
}


void Zebra_Im::stopZebra()
{
	if(isRun) {
		stop();
		isRun = false;
	}
}


int Zebra_Im::send(const string &id, uchar *buf, int len)
{
	int ret = 0;
	if(isRun && id.length()) {
		string data((char*)buf,len);
		ret = send_message(id, data);
	}

	return ret;
}


void Zebra_Im::rebootZebra()
{
	Zebra_Cfg *cfg = Zebra_Cfg::bulid();
	if(cfg->channel_id.length()) {
//		destroyChannel(cfg->channel_id);
	}
	cfg->join_id = "";
	cfg->channel_id = "";
}
