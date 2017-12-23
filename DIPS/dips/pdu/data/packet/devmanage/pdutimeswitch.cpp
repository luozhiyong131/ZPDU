/*
 * pdutimeswitch.cpp
 * PDU设备定时开关功能
 *  主要使用2个map表
 *      1、没有分组的输出位，使用一个map表，
 *      2、根据组名，操作一个map表，
 *
 *   注意：当组有变动时，请调用del()方法重新加载数据
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdutimeswitch.h"

PduTimeSwitch::PduTimeSwitch()
{
    mLock = new QReadWriteLock();
}

PduTimeSwitch::~PduTimeSwitch()
{
    del();
    delete mLock;
}


/**
 * @brief 清理所有定时数据， 当分组发生改变时，一定要调用此函数清理数据，保证定时信息同步
 */
void PduTimeSwitch::del(void)
{
    QWriteLocker locker(mLock);
    mMap.clear();
    mGroupMap.clear();
}


/**
 * @brief 设置定时时间
 * @param output
 * @param time
 */
void PduTimeSwitch::set(int output,const QString &time)
{
    QWriteLocker locker(mLock);
    bool ret = mMap.contains(output);
    if(ret)
        mMap[output] = time;
    else
        mMap.insert(output, time);
}


/**
 * @brief 获取定时器时间
 * @param output
 * @param time
 * @return
 */
QString PduTimeSwitch::get(int output) //获取输出位名称
{
    QString time;

    QReadLocker locker(mLock);
    bool ret = mMap.contains(output);
    if(ret)
        time = mMap[output];

    return time;
}


int PduTimeSwitch::size(void) // 输出位的位数
{
     QReadLocker locker(mLock);
     return mMap.size();
}



int PduTimeSwitch::sizeGroup(void) // 输出位的位数
{
    QReadLocker locker(mLock);
    return mGroupMap.size();
}



/**
 * @brief 设置定时时间
 * @param output
 * @param time
 */
void PduTimeSwitch::setGroup(const QString &group,const QString &time)
{
    QWriteLocker locker(mLock);
    bool ret = mGroupMap.contains(group);
    if(ret)
        mGroupMap[group] = time;
    else
        mGroupMap.insert(group, time);
}


/**
 * @brief 获取定时器时间
 * @param output
 * @param time
 * @return
 */
QString PduTimeSwitch::getGroup(const QString &group) //获取输出位名称
{
    QString time;

    QReadLocker locker(mLock);
    bool ret = mGroupMap.contains(group);
    if(ret)
        time = mGroupMap[group];

    return time;
}
