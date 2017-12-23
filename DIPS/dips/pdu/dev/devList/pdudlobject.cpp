/*
 * pdudevlist.cpp
 *  PDU设备列表
 *  1、根据IP地址，列出级联中主、副机的，在线、离线、报警的设备号
 *  2、设备号已进行排序
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudlobject.h"

PduDLObject::PduDLObject()
{
}

PduDLObject::~PduDLObject()
{
}



/**
 * @brief 设备列表
 * @param data
 * @return
 */
int PduDLObject::list(QString &ip, QList<int> &numList)
{
    return get(ip, PDU_DL_LIST, numList);
}

/**
 * @brief 报警设备
 * @param data
 * @return
 */
int PduDLObject::alarm(QString &ip, QList<int> &numList)
{
    return get(ip, PDU_DL_ALARM, numList);
}

/**
 * @brief 在线设备
 * @param data
 * @return
 */
int PduDLObject::line(QString &ip, QList<int> &numList)
{
     return get(ip, PDU_DL_LINE, numList);
}

/**
 * @brief 离线设备
 * @param data
 * @return
 */
int PduDLObject::offLine(QString &ip, QList<int> &numList)
{
    return get(ip, PDU_DL_OFF, numList);
}



void PduDLObject::add(PduDataPacket *data,int id, QList<int> &numList)
{
    int num = -1;

    switch (id)
    {
    case PDU_DL_LIST: // 设备列表
        num = data->devNum;
        break;

    case PDU_DL_LINE:
        if(data->offLine > 0)  // 设备在线
             num = data->devNum;
        break;

    case PDU_DL_OFF: // 离线设备
        if(data->offLine < 1)
             num = data->devNum;
        break;

    case PDU_DL_ALARM: // 报警设备
        if(data->state)
             num = data->devNum;
        break;

    default:
        break;
    }

    if(num >= 0)
    {
        bool ret = numList.contains(num);
        if(ret == false)
            numList << num;
    }
}


int PduDLObject::get(QString &ip, int id, QList<int> &numList)
{
    PduDevHash *devHash = pdu_hd_get(ip);
    if(devHash)
    {
        QList<int> devList;
        devHash->list(devList); // 根据设备IP，获取设备地址列表 级联

        for(int k=0; k<devList.size(); ++k)
        {
            PduDataPacket *data = devHash->get(devList.at(k)); // 获取设备数据
            if(data)
                add(data, id, numList);
        }
        qSort(numList); // 数据排序
    }   

    return numList.size();
}
