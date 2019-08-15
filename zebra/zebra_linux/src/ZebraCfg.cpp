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
}


Zebra_Cfg *Zebra_Cfg::bulid()
{
	static Zebra_Cfg* sington = new Zebra_Cfg();;
	return sington;
}

