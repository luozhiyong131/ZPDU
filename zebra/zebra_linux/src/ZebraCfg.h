/*
 * ZebraCfg.h
 *
 *  Created on: 2019年8月9日
 *      Author: Lzy
 */

#ifndef ZEBRACFG_H_
#define ZEBRACFG_H_
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char uchar;
typedef unsigned int uint;
using namespace std;


class Zebra_Cfg
{
	Zebra_Cfg();
public:
	 static Zebra_Cfg *bulid();

	 bool dhtStatus;
	 string node_id;
	 string join_id;
	 string channel_id;
};

#endif /* ZEBRACFG_H_ */
