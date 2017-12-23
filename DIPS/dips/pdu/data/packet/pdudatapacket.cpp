/*
 * pdudatapacket.cpp
 * PDU设备数据包
 *      1、包含整个设备数据信息
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudatapacket.h"

PduDataPacket::PduDataPacket()
{
    devNum = -1;
    devType = -1;
    state = 0;
    offLine = 0;
    devSpec = 0;
    txType = 0;
    sqlId = -1;

    ip = new PduStrBase(); //设备IP
    info = new PduDevInfo(); // 设备型号、设备地址

    data = new PduDevData(); //设备数据
    output = new PduOutput(); //输出位信息

    usr = new PduUsrManager(); //设备用户
    net = new PduNetInfo(); //设备网络信息

    manage = new PduDevManage(); //设备管理信息
    chart = new PduDevChart(); //设备图表信息

    clearFlag = true;
}

PduDataPacket::~PduDataPacket()
{
    delete ip;
    delete info;

    delete data;
    delete output;

    delete usr;
    delete net;

    delete manage;
    delete chart;
}
