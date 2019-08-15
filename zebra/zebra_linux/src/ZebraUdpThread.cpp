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

#define UDP_BUF_SIZE 256
#define UDP_DEST_PORT 27071
static struct sockaddr_in addr_serv;
static char server_ip[32] = {"192.168.1.215"}; /*服务器IP地址*/


Zebra_UdpThread::Zebra_UdpThread()
{
	mChannel = new Zebra_Channel();
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



/**
 * 功能：创建UDP服务端套接字
 * 返回 ：套接字
 */
int Zebra_UdpThread::serviceSocket(int port)
{
	int sockfd;
	struct sockaddr_in server_addr;/* 主机IP地址和端口号 */

	/* 创建一个socket，类型是SOCK_DGRAM，UDP类型 */
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		printf("Socket error\n");
		return -1;
	}

	/* 初始化服务端地址 */
	server_addr.sin_family = AF_INET;		/*IPv4因特网域*/
	server_addr.sin_port = htons(port);    /*端口号，这里进行网络字节序的转换 */
	//	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

	/* 绑定socket到服务端地址 */
	if (bind(sockfd, (struct sockaddr *)&server_addr,
			sizeof(struct sockaddr)) == -1)
	{
		/* 绑定地址失败 */
		printf("Bind error\n");
		return -1;
	}
//	printf("UDP Server Waiting for client on port %d...\n", port);

	return sockfd;
}

int Zebra_UdpThread::serverIP(struct sockaddr_in *client_addr)
{
	memset(server_ip, 0, sizeof(server_ip));
	sprintf(server_ip,"%s", inet_ntoa(client_addr->sin_addr)); /*获取服务端IP*/
	//	printf("Server:(%s , %d)\n", server_ip,ntohs(client_addr->sin_port));

	return 0;
}

/**
 * 功  能：UDP 数据接收
 * 出口参数：*recv_data -> 接收数据缓冲区
 * 返  回: 数据长度
 */
int Zebra_UdpThread::serviceRecvData(int sockfd,struct sockaddr_in *client_addr,char *recv_data)
{
	int ret=0;
	uint addr_len = sizeof(struct sockaddr);

	/* 从sock中收取最大BUF_SIZE - 1字节数据 */
	/* UDP不同于TCP，它基本不会出现收取的数据失败的情况，除非设置了超时等待 */
	ret = recvfrom(sockfd, recv_data, UDP_BUF_SIZE - 1, 0,
			(struct sockaddr *)client_addr, &addr_len);
//		printf("UDP Recv %s Data len:%d %s\n",server_ip, ret, recv_data);

	return ret;
}


void Zebra_UdpThread::recv()
{
	int rtn=0;
	char buf[UDP_BUF_SIZE] = {0};
	struct sockaddr_in clientAddr;
	int sock_fd = serviceSocket(UDP_DEST_PORT+1);

	while(1) {
		memset(buf, 0, UDP_BUF_SIZE);
		rtn = serviceRecvData(sock_fd, &clientAddr, buf);
		if(rtn > 0) {
			Zebra_Im::bulid()->rebootZebra();
//			  serverIP(client_addr);
		}
	}
}

int Zebra_UdpThread::getSendStr(char *ptr)
{
	Zebra_Cfg *cfg = Zebra_Cfg::bulid();

	ptr[0] = 0;
	if(cfg->node_id.length() && cfg->dhtStatus)
	{
		if((cfg->join_id.length()) || (cfg->channel_id.length()))
			sprintf(ptr, "node_id = %s; join_id = %s; channel_id = %s",
					cfg->node_id.c_str(), cfg->join_id.c_str(), cfg->channel_id.c_str());
	}

	return strlen(ptr);
}

int Zebra_UdpThread::workDown(int sock_fd)
{
	static char send_buf[512];
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
		const int opt = 1;
		setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
		memset(&addr_serv, 0, sizeof(addr_serv));
		addr_serv.sin_family = AF_INET;
		addr_serv.sin_addr.s_addr=htonl(INADDR_BROADCAST);
		addr_serv.sin_port = htons(UDP_DEST_PORT);
	} else {
		perror("socket");
	}

	return sock_fd;
}



void Zebra_UdpThread::run()
{
	int sock_fd = createSocket();
	if(sock_fd < 0) return ;

	while(1) {
		sleep(5);
		mChannel->createChannel();
		workDown(sock_fd);
	}
}

static void* zebra_udp_entry(void* args)
{
	Zebra_UdpThread *udp = Zebra_UdpThread::bulid();
	udp->run();

	return 0;
}



static void* zebra_udprcv_entry(void* args)
{
	Zebra_UdpThread *udp = Zebra_UdpThread::bulid();
	udp->recv();

	return 0;
}

int zebra_udp_thread()
{
	pthread_t tids;
	int ret = pthread_create(&tids, NULL, zebra_udp_entry, NULL);
	if (ret != 0) {
		cout << "pthread_udp_create error: error_code=" << ret << endl;
	}

	ret = pthread_create(&tids, NULL, zebra_udprcv_entry, NULL);
	if (ret != 0) {
		cout << "pthread_udp_rcv_create error: error_code=" << ret << endl;
	}

	return ret;
}



