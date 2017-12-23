/*
 * pduhaship.cpp
 * PDU设备Hash表
 *  主要以设备IP，保存PduDevHash对象
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduhaship.h"

#ifdef ZEBRA_MODULE
#include "autoaddfriend/autoaddfriend.h"
#endif

PduHashIP::PduHashIP()
{
    mLock = new QReadWriteLock();
}

PduHashIP::~PduHashIP()
{
    delete mLock;
    mHash.clear();
}

/**
 * @brief 根据IP来增加一个表
 * @param ip
 */
void PduHashIP::add(const QString &ip)
{
    PduDevHash *dev = new PduDevHash();

    QWriteLocker locker(mLock);
    mHash.insert(ip, dev); //增加一个设备
}

/**
 * @brief 大小
 * @return
 */
int PduHashIP::size(void)
{
    QReadLocker locker(mLock);
    return mHash.size();
}

/**
 * @brief 删除
 * @param ip
 * @return
 */
bool PduHashIP::del(const QString &ip)
{
    bool ret = mHash.contains(ip);
    if(ret)
    {
        QWriteLocker locker(mLock);
        mHash.remove(ip);
    }

    return ret;
}


PduDevHash *PduHashIP::getDev(const QString &ip)
{
    QReadLocker locker(mLock);
    QHash<QString,PduDevHash *>::const_iterator it;
    it = mHash.constFind(ip);
    if(it != mHash.constEnd())
        mDev = it.value();
    else
        mDev = NULL;

    return mDev;
}




/**
 * @brief 根据IP获取对应的指针 设备不存在就增加设备
 * @param ip
 * @return
 */
PduDevHash * PduHashIP::get(const QString &ip)
{
#ifdef ZEBRA_MODULE
    AutoAddFriend ad_tmp;
    ad_tmp.addFriend(ip);
#endif
    bool ret = mHash.contains(ip); //此设备是否存在
    if(ret == false)
    {
        add(ip); //增加设备
    }

    return getDev(ip);
}

int PduHashIP::list(QStringList &list)
{
    QReadLocker locker(mLock);
    QHash<QString, PduDevHash *>::const_iterator i;
    for(i=mHash.constBegin(); i!= mHash.constEnd(); ++i)
        list << i.key();

    return mHash.size();
}
