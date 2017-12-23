/*
 * pdudatabase.cpp
 * 数据链表，PDU数据的最小组成单元
 *  1、数据使用读写锁
 *  2、使能List实现数组链表
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudatabase.h"

#define NET_DATA_BASE_SIZE 100

PduDataBase::PduDataBase()
{
    mLock = new QReadWriteLock();
    mDataList = new QList<int>();
}

PduDataBase::~PduDataBase()
{
    delete mLock;
    delete mDataList;
}

/**
 * @brief 增加数据
 * @param data
 */
void PduDataBase::add(int data)
{
    QWriteLocker locker(mLock);
    mDataList->append(data);
}

/**
 * @brief 链表大小
 * @return
 */
int PduDataBase::size(void)
{
    QReadLocker locker(mLock);
    return mDataList->size();
}

/**
 * @brief 设置值
 * @param local 位置
 * @param data 数据
 * @return true
 */
bool PduDataBase::set(int local, int data)
{
    bool ret = true;
    if(local < NET_DATA_BASE_SIZE) // 必须小于
    {
        while (local >= size()) // 数组没这么大，扩大数组补0
            add(0);

        if(local < size())
        {
            QWriteLocker locker(mLock);
            mDataList->replace(local, data);
        }
    }
    else
        ret = false;

    return ret;
}


/**
 * @brief 获取数据
 * @param local
 * @return
 */
int PduDataBase::get(int local)
{
    int data = -1;

    if(local < size())
    {
        QReadLocker locker(mLock);
        data = mDataList->at(local);
    }

    return data;
}

/**
 * @brief 统一修改
 * @param data
 * @return
 */
void PduDataBase::setAll(int data)
{
    QWriteLocker locker(mLock);

    int size = mDataList->size();
    for(int i=0; i<size; ++i)
        mDataList->replace(i, data);
}
