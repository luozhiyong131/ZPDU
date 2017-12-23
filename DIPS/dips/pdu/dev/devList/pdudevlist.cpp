/*
 * pdudevlist.cpp
 *  PDU设备列表
 *  1、列出在线、离线、报警的设备的IP地址
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudevlist.h"

PduDevList::PduDevList()
{
}

PduDevList::~PduDevList()
{
}



/**
 * @brief 获取设备列表
 * @return
 */
int PduDevList::getList(int type, QStringList &ipList)
{
    return get(type, PDU_DL_LIST, ipList);
}

/**
 * @brief 获取在线设备列表
 * @return
 */
int PduDevList::getLine(int type, QStringList &ipList)
{
    return get(type, PDU_DL_LINE, ipList);
}

/**
 * @brief 获取报警设备列表
 * @return
 */
int PduDevList::getAlarm(int type, QStringList &ipList)
{
    return get(type, PDU_DL_ALARM, ipList);
}

/**
 * @brief 获取高线设备列表
 * @return
 */
int PduDevList::getOff(int type, QStringList &ipList)
{
    return get(type, PDU_DL_OFF, ipList);
}


int PduDevList::get(int type, int id, QStringList &list)
{
    PduHashIP *hashIP =  pdu_hashData_getHash(type); // 获取某种设备类型的对象
    QStringList ipList;
    hashIP->list(ipList); // 根据设备类型，获取设备IP列表

    for(int j=0; j<ipList.size(); ++j)
    {
        PduDevHash *devHash = hashIP->getDev(ipList.at(j));// 获取设备对象
        QList<int> devList;
        devHash->list(devList); // 根据设备IP，获取设备地址列表 级联

        for(int k=0; k<devList.size(); ++k)
        {
            PduDataPacket *data = devHash->getPacket(devList.at(k)); // 获取设备数据
            add(data,id, list);
        }
    }

    return ipList.size();
}


void PduDevList::add(PduDataPacket *data,int id, QStringList &ipList)
{
    QString ip;

    switch (id)
    {
    case PDU_DL_LIST: // 设备列表
        ip = data->ip->get();
        break;

    case PDU_DL_LINE:
        if(data->offLine > 0)  // 设备在线
            ip = data->ip->get();
        break;

    case PDU_DL_OFF: // 离线设备
        if(data->offLine < 1)
            ip = data->ip->get();
        break;

    case PDU_DL_ALARM: // 报警设备
        if(data->state)
            ip = data->ip->get();
        break;

    default:
        break;
    }

    if(!ip.isEmpty())
    {
        bool ret = ipList.contains(ip);
        if(ret == false)
            ipList << ip;
    }
}
