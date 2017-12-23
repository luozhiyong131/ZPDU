/*
 * UdpSocketList.cpp
 * UDP 接收套接字
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "udpsocketlist.h"

UdpSocketList::UdpSocketList(QObject *parent) : QThread(parent)
{
    isRun = false;
    mSocketList = new QList<UdpRecvSocket *>();
}


UdpSocketList::~UdpSocketList()
{
    mSocketList->clear();
    delete mSocketList;
}



/**
 * @brief 初始化UDP监听端口号，
 * @param num　创建端口个数
 */
void UdpSocketList::initSocket(int num)
{
    int port=0;
    UdpRecvSocket *socket = NULL;

    for(int i=0; i<num; ++i)
    {
        socket = new UdpRecvSocket(this);
        port = socket->initSocket();
        mSocketList->append(socket); // 保存套接字

        udp_port_add(port); // 增加端口号
    }

    start(); // 启动线程
}

void UdpSocketList::stopAndWaiting()
{
    isRun = false;
    wait();
}

/**
 * @brief 读取套接字中的数据
 */
int UdpSocketList::recvData(void)
{
    int ret = 0;

    for(int i=0; i<mSocketList->size(); ++i)
    {
        mSocket = mSocketList->at(i);
        mData = mSocket->getData();
        if(mData)
        {
             udp_data_add(mData);
             ret++;
        }
    }

    return ret;
}



void UdpSocketList::run(void)
{
    int ret;
    isRun = true;
    while(isRun){
        ret = recvData();
        if(ret <= 0)
            usleep(1);
    }
}




