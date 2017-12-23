/*
 * pdustrbase.cpp
 * 字符串基类，字符串最小组成单元
 *  1、字符串使用读写锁
 *  2、修改之前，先比较字符串是否需要修改，字符串不相等才修改
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdustrbase.h"

PduStrBase::PduStrBase()
{
    mLock = new QReadWriteLock();
}


PduStrBase::~PduStrBase()
{
    mStr.clear();
    delete mLock;
}


/**
 * @brief 修改字符串
 * @param str
 */
void PduStrBase::set(const QString &str)
{
    QWriteLocker locker(mLock);
    int ret = QString::compare(mStr, str);
    if(ret)
        mStr = str;
}

/**
 * @brief 获取字符串
 * @param str
 */
void PduStrBase::get(QString &str)
{
    QReadLocker locker(mLock);
    str = mStr;
}

/**
 * @brief 获取字符串
 * @return
 */
QString PduStrBase::get(void)
{
    QReadLocker locker(mLock);
    return mStr;
}

/**
 * @brief 清除内容
 */
void PduStrBase::clear(void)
{
    QWriteLocker locker(mLock);
    mStr.clear();
}
