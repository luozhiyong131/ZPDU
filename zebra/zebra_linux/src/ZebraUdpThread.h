/*
 * ZebraUdpThread.h
 *
 *  Created on: 2019年8月9日
 *      Author: Lzy
 */

#ifndef ZEBRAUDPTHREAD_H_
#define ZEBRAUDPTHREAD_H_
#include "ZebraIm.h"
#include <pthread.h>


class Zebra_UdpThread
{
	Zebra_UdpThread();
public:
	virtual ~Zebra_UdpThread();
	static Zebra_UdpThread *bulid();

	void run();
protected:
	int createSocket();
	int getSendStr(char *ptr);
	int workDown(int sock_fd);
};
int zebra_udp_thread();

#endif /* ZEBRAUDPTHREAD_H_ */
