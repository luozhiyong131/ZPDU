/*
 * pducutoff.cpp
 * PDU设备超限断电功能
 *  主要使用2个map表
 *      1、没有分组的输出位，使用一个map表，
 *      2、根据组名，操作一个map表，
 *
 *   注意：当组有变动时，请调用del()方法重新加载数据
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pducutoff.h"

PduCutOff::PduCutOff()
{
    mLock = new QReadWriteLock();
}

PduCutOff::~PduCutOff()
{
    del();
    delete mLock;
}


/**
 * @brief 清理所有定时断电数据， 当分组发生改变时，一定要调用此函数清理数据，信息同步
 */
void PduCutOff::del(void)
{
    QWriteLocker locker(mLock);
    mMap.clear();
    mGroupMap.clear();
}


/**
 * @brief 设置定时断电
 * @param output
 * @param time
 */
void PduCutOff::set(int output,bool en)
{
    QWriteLocker locker(mLock);
    bool ret = mMap.contains(output);
    if(ret)
            mMap[output] = en;
    else
        mMap.insert(output, en);
}

/**
 * @brief 全开、全关
 * @param en
 */
void PduCutOff::setAll(bool en)
{
    QWriteLocker locker(mLock);
    QMap<int, bool>::const_iterator i;
    for(i=mMap.constBegin(); i!= mMap.constEnd(); ++i)
    {
        int output = i.key();
        mMap[output] = en; // 自动增加后缀
    }
}

/**
 * @brief 获取定时断电
 * @param output
 * @param time
 * @return
 */
bool PduCutOff::get(int output, bool &en)
{
    QReadLocker locker(mLock);
    bool ret = mMap.contains(output);
    if(ret)
        en = mMap.value(output);

    return ret;
}


int PduCutOff::size(void) // 输出位的位数
{
     QReadLocker locker(mLock);
     return mMap.size();
}


void PduCutOff::setGroup(const QString &group,bool en)
{
    QWriteLocker locker(mLock);
    bool ret = mGroupMap.contains(group);
    if(ret)
        mGroupMap[group] = en;
    else
        mGroupMap.insert(group, en);
}



bool PduCutOff::getGroup(const QString &group, bool &en) //获取输出位名称
{
    QReadLocker locker(mLock);
    bool ret = mGroupMap.contains(group);
    if(ret)
        en = mGroupMap.value(group);

    return ret;
}

int PduCutOff::sizeGroup(void) // 输出位的位数
{
    QReadLocker locker(mLock);
    return mGroupMap.size();
}

