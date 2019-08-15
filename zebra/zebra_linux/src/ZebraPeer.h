/*
 * ZebraPeer.h
 *
 *  Created on: 2019年8月12日
 *      Author: Lzy
 */

#ifndef SRC_ZEBRAPEER_H_
#define SRC_ZEBRAPEER_H_

#include "ZebraUdpThread.h"

class Zebra_Peer
{
	Zebra_Peer();
public:
	virtual ~Zebra_Peer();
	static Zebra_Peer *bulid();

	int send(uchar *buf, int len);

private:
    Zebra_Im *mIm;
};

#endif /* SRC_ZEBRAPEER_H_ */
