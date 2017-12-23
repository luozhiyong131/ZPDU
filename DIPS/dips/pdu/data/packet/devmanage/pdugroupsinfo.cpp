/*
 * pdugroupsinfo.cpp
 * PDU设备输出位分组信息
 *  主要使用map 依据输出位，来保存输出位所属组名
 *      1、可以设置输出名组名
 *      2、根据输出位获取组名
 *      3、获取组名列表
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdugroupsinfo.h"

PduGroupsInfo::PduGroupsInfo()
{
    mLock = new QReadWriteLock();
}

PduGroupsInfo::~PduGroupsInfo()
{
    delete mLock;
    mMap.clear();
}


void PduGroupsInfo::del(void)
{
    QWriteLocker locker(mLock);
    mMap.clear();
}


/**
 * @brief 根据输出位，设置组名
 * @param output
 * @param groupName
 */
void PduGroupsInfo::set(int output,const QString &groupName)
{
    QWriteLocker locker(mLock);
    bool ret = mMap.contains(output);
    if(ret)
    {
        if(mMap.value(output) != groupName)
            mMap[output] = groupName;
    }
    else
        mMap.insert(output, groupName);
}

/**
 * @brief 根据输出位，获取组名
 * @param output
 * @param groupName
 * @return
 */
bool PduGroupsInfo::get(int output, QString &groupName) //获取组名
{
    QReadLocker locker(mLock);
    bool ret = mMap.contains(output);
    if(ret)
        groupName = mMap.value(output);

    return ret;
}

/**
 * @brief 获取组列表
 * @param group
 * @return
 */
int PduGroupsInfo::groupList(QStringList &group) // 获取组名列表
{
    QString str;

    QReadLocker locker(mLock);
    QMap<int, QString>::const_iterator i;
    for(i=mMap.constBegin(); i!= mMap.constEnd(); ++i)
    {
        str = i.value();

        bool ret = group.contains(str);
        if(ret == false)
            group << str;
    }

    return group.size();
}

/**
 * @brief 根据组名获取输出位列表
 * @param groupName
 * @param array
 * @return
 */
int PduGroupsInfo::group(const QString &groupName, int array[])
{
    int offset = 0;
    QString str;

    QReadLocker locker(mLock);
    QMap<int, QString>::const_iterator i;
    for(i=mMap.constBegin(); i!= mMap.constEnd(); ++i)
    {
        str = i.value();
        if(str == groupName)
            array[offset++] = i.key();
    }

    return offset;
}
