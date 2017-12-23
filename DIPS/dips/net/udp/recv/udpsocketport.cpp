/*
 * udpsocketport.cpp
 * UDP 端口号链表
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "udpsocketport.h"
static QList<int> gUDPSocketPort; /*UDP监听端口链表*/

/**
 * @brief 增加UDP端口号
 * @param port 端口号
 */
void udp_port_add(int port)
{
    gUDPSocketPort.append(port);
}

/**
 * @brief 获取端口号的个数
 * @return  端口号总个数
 */
int udp_port_size(void)
{
    return gUDPSocketPort.count();
}


/**
 * @brief 获得端口号
 * @param i
 * @return
 */
int udp_port_get(int i)
{
    int port = -1;
    if(i<udp_port_size())
        port = gUDPSocketPort.at(i);

    return port;
}

/**
 * @brief 清除
 */
void udp_port_clear(void)
{
    gUDPSocketPort.clear();
}

