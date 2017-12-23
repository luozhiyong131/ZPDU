/*
 * pduhashdata.cpp
 * PDU设备Hash表
 *  此Hash为全局变量
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduhashdata.h"

static QReadWriteLock *gLock = new QReadWriteLock();
static QHash<int,PduHashIP *> *gPduDataHash = new QHash<int,PduHashIP *>; //设备Hash表
static QHash<int,PduHashIP *> *gHash = gPduDataHash;


/**
 * @brief 根据设备代号，增加设备类型
 * @param type 设备类型代号
 */
static void pdu_hashData_add(int type)
{
    PduHashIP *pdu = new PduHashIP();

    QWriteLocker locker(gLock);
    gHash->insert(type, pdu); //增加一种类型
}

/**
 * @brief 获取设备种类的数量
 * @return
 */
int pdu_hashData_size(void)
{
    QReadLocker locker(gLock);
    return gHash->size();
}

/**
 * @brief 删除一种类型的设备
 * @param type
 * @return
 */
bool pdu_hashData_del(int type)
{
    bool ret = gHash->contains(type);
    if(ret)
    {
        QWriteLocker locker(gLock);
        gHash->remove(type);
    }

    return ret;
}

PduHashIP *pdu_hashData_getHash(int type)
{
    PduHashIP *pduHash = NULL;

    QReadLocker locker(gLock);
    QHash<int,PduHashIP *>::const_iterator it;
    it = gHash->constFind(type);
    if(it != gHash->constEnd())
        pduHash = it.value();

    return pduHash;
}


/**
 * @brief 获取一种设备
 * @param type
 * @return
 */
PduHashIP *pdu_hashData_get(int type)
{
    bool ret = gHash->contains(type); //此设备是否存在
    if(ret == false)
    {
        pdu_hashData_add(type); //增加设备
    }

    return pdu_hashData_getHash(type);
}


int pdu_hashData_list(QList<int> &list)
{
    QReadLocker locker(gLock);
    QHash<int, PduHashIP *>::const_iterator i;
    for(i=gHash->constBegin(); i!= gHash->constEnd(); ++i)
        list << i.key();
    qSort(list);

    return gHash->size();
}

