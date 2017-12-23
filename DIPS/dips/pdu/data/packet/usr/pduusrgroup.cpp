/*
 * pduusrgroup.cpp
 * PDU设备用户组
 *  其中使用Hash表，以组名为Key,权限为value 进行管理
 *
 *   注意：当组有变动时，请调用del()方法重新加载数据
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduusrgroup.h"

PduUsrGroup::PduUsrGroup()
{
    mLock = new QReadWriteLock();
}

PduUsrGroup::~PduUsrGroup()
{
    del();
    delete mLock;
}

/**
 * @brief 清空
 */
void PduUsrGroup::del(void)
{
    QWriteLocker locker(mLock);
    mHash.clear();
}

/**
 * @brief 删除组
 * @param group
 */
void PduUsrGroup::del(const QString &group)
{
    QWriteLocker locker(mLock);
    bool ret = mHash.contains(group);
    if(ret)
        mHash.remove(group);
}

/**
 * @brief 增加或修改组
 * @param group
 * @param data
 */
void PduUsrGroup::set(const QString &group, const int data)
{
    QWriteLocker locker(mLock);
    bool ret = mHash.contains(group);
    if(ret)
        mHash[group] = data;
    else
        mHash.insert(group, data);
}

/**
 * @brief 大小
 * @return
 */
int PduUsrGroup::size(void)
{
    QReadLocker locker(mLock);
    return mHash.size();
}

/**
 * @brief 获取组列表
 * @param group
 * @return
 */
int PduUsrGroup::getGroup(QStringList &group)
{
    QReadLocker locker(mLock);
    QHash<QString, int>::const_iterator i;
    for(i=mHash.constBegin(); i!= mHash.constEnd(); ++i)
        group << i.key();

    return mHash.size();
}

/**
 * @brief 获取组信息
 * @param group
 * @param data
 * @return
 */
bool PduUsrGroup::get(const QString &group, int &data)
{
    bool ret = true;

    QReadLocker locker(mLock);
    QHash<QString, int>::const_iterator it;
    it = mHash.constFind(group);
    if(it != mHash.constEnd())
        data = it.value();
    else
        ret = false;

    return ret;
}













