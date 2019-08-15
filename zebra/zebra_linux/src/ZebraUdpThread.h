/*
 * ZebraUdpThread.h
 *
 *  Created on: 2019年8月9日
 *      Author: Lzy
 */

#ifndef ZEBRAUDPTHREAD_H_
#define ZEBRAUDPTHREAD_H_
#include "ZebraChannel.h"
#include <pthread.h>

class Zebra_UdpThread
{
	Zebra_UdpThread();
public:
	virtual ~Zebra_UdpThread();
	static Zebra_UdpThread *bulid();

	void recv();
	void run();

protected:
	int createSocket();
	int getSendStr(char *ptr);
	int workDown(int sock_fd);

	int serviceSocket(int port);
	int serviceRecvData(int sockfd,struct sockaddr_in *client_addr,char *recv_data);
	int serverIP(struct sockaddr_in *client_addr);

private:
	Zebra_Channel *mChannel;
};
int zebra_udp_thread();

#endif /* ZEBRAUDPTHREAD_H_ */
