/*
 * ZebraUdpThread.cpp
 *
 *  Created on: 2019年8月9日
 *      Author: Lzy
 */

#include "ZebraUdpThread.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEST_PORT 27071
#define DSET_IP_ADDRESS  "255.255.255.255"
static struct sockaddr_in addr_serv;


Zebra_UdpThread::Zebra_UdpThread()
{

}

Zebra_UdpThread::~Zebra_UdpThread()
{

}

Zebra_UdpThread *Zebra_UdpThread::bulid()
{
	static Zebra_UdpThread* sington = NULL;
	if(sington == NULL) {
		sington = new Zebra_UdpThread();
	}

	return sington;
}



int Zebra_UdpThread::getSendStr(char *ptr)
{
	Zebra_Cfg *cfg = Zebra_Cfg::bulid();

	ptr[0] = 0;
	if(cfg->node_id.length()) {
		if((cfg->join_id.length()) || (cfg->channel_id.length()))
			sprintf(ptr, "node_id = %s; join_id = %s; channel_id = %s",
					cfg->node_id.c_str(), cfg->join_id.c_str(), cfg->channel_id.c_str());
	}

	return strlen(ptr);
}

int Zebra_UdpThread::workDown(int sock_fd)
{
	static char send_buf[256];
	char *ptr = send_buf;

	int ret = getSendStr(ptr);
	if(ret) {
		ret = sendto(sock_fd, ptr, strlen(ptr), 0, (struct sockaddr *)&addr_serv, sizeof(addr_serv));
	}

	return ret;
}

int Zebra_UdpThread::createSocket()
{
	/* socket文件描述符 */
	int sock_fd  = socket(AF_INET, SOCK_DGRAM, 0); /* 建立udp socket */
	if(sock_fd > 0) {
		memset(&addr_serv, 0, sizeof(addr_serv));
		addr_serv.sin_family = AF_INET;
		addr_serv.sin_addr.s_addr = inet_addr(DSET_IP_ADDRESS);
		addr_serv.sin_port = htons(DEST_PORT);
	} else {
		perror("socket");
	}

	return sock_fd;
}

void Zebra_UdpThread::run()
{
	int sock_fd = createSocket();
	if(sock_fd) return ;

	while(1) {
		sleep(5);
		workDown(sock_fd);
	}
}



static void* zebra_udp_entry(void* args)
{
	Zebra_UdpThread *udp = Zebra_UdpThread::bulid();
	udp->run();

	return 0;
}

int zebra_udp_thread()
{
	pthread_t tids;
	int ret = pthread_create(&tids, NULL, zebra_udp_entry, NULL);
	if (ret != 0) {
		cout << "pthread_create error: error_code=" << ret << endl;
	}
	return ret;
}



