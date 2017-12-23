/*
 * pdudevhash.cpp
 * PDU设备Hash表
 *  主要以设备号，保存设备数据
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudevhash.h"

PduDevHash::PduDevHash()
{
    mPacket = NULL;
    mLock = new QReadWriteLock();
}

PduDevHash::~PduDevHash()
{
    delete mLock;
    mHash.clear();
}

/**
 * @brief 根据设备号，增加一个设备的数据
 * @param num
 */
void PduDevHash::add(int num)
{
    PduDataPacket *packet = new PduDataPacket();
    packet->devNum = num; //设置设备号

    QWriteLocker locker(mLock);
    mHash.insert(num, packet); //增加一个设备
}

/**
 * @brief 获取设备数据
 * @return N 表示级联台数N-1
 */
int PduDevHash::size(void)
{
    QReadLocker locker(mLock);
    return mHash.size();
}

/**
 * @brief 删除一个设备
 * @param num
 * @return
 */
bool PduDevHash::del(int num)
{
    bool ret = mHash.contains(num);
    if(ret)
    {
        QWriteLocker locker(mLock);
        mHash.remove(num);
    }

    return ret;
}

/**
 * @brief 获取数据包
 * @param num
 * @return
 */
PduDataPacket *PduDevHash::getPacket(int num)
{
    QReadLocker locker(mLock);
    QHash<int,PduDataPacket *>::const_iterator it;
    it = mHash.constFind(num);
    if(it != mHash.constEnd())
        mPacket = it.value();
    else
        mPacket = NULL;

    return mPacket;
}

/**
 * @brief 获取一个设备的 数据指针
 * @param num
 * @return
 */
PduDataPacket *PduDevHash::get(int num)
{
    bool ret = mHash.contains(num); //此设备是否存在
    if(ret == false)
    {
        add(num); //增加设备
        isNew = true; // 新设备
    }
    else
        isNew = false;

   return getPacket(num);
}




/**
 * @brief 获得主机数据指针
 * @return
 */
PduDataPacket *PduDevHash::getMaster(void)
{
    PduDataPacket *packet = getPacket(0);
   // if(packet->offLine < 1)
   //     packet = NULL;

    return packet;
}

int PduDevHash::list(QList<int> &list)
{
    QReadLocker locker(mLock);
    QHash<int, PduDataPacket *>::const_iterator i;
    for(i=mHash.constBegin(); i!= mHash.constEnd(); ++i)
        list << i.key();

    return mHash.size();
}
