/*
 * ZebraCfg.h
 *
 *  Created on: 2019年8月9日
 *      Author: Lzy
 */

#ifndef ZEBRACFG_H_
#define ZEBRACFG_H_
#include "cfg_op.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char uchar;
typedef unsigned int uint;
using namespace std;

struct Zebra_sNodeItem {
    string node_id;
    string public_key;
    string private_key;
};

struct Zebra_sChItem
{
    int expire, type;
    string channel_id , ch_owner_id;
    string public_key, private_key, secret;
    string f_node_id, f_pubkey, f_privatekey;
    string t_node_id, t_pubkey, t_privatekey;
};

class Zebra_Cfg
{
	Zebra_Cfg();
public:
	 static Zebra_Cfg *bulid();
	 virtual ~Zebra_Cfg();

	 bool dhtStatus;
	 string node_id;
	 string join_id;
	 string channel_id;

	 void saveChannel();
	 void readChannel();
	 Zebra_sChItem *getChannel();

	 Zebra_sNodeItem *findNode(string &node);
	 void updateNode(Zebra_sNodeItem &node);

	 void saveNode();
	 void readNode();

private:
	Zebra_sNodeItem *mNode;
	Zebra_sChItem *mChannel;
};

#endif /* ZEBRACFG_H_ */
