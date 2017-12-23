/*
 * pduoutputsort.cpp
 * PDU设备输出位排序信息
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduoutputsort.h"

PduOutputSort::PduOutputSort()
{
    mLock = new QReadWriteLock();

}

PduOutputSort::~PduOutputSort()
{
    delete mLock;
    mMap.clear();
}

/**
 * @brief 设置输出位序号
 * @param output
 * @param name
 */
void PduOutputSort::set(int output,const int value)
{
    QWriteLocker locker(mLock);
    bool ret = mMap.contains(output);
    if(ret)
            mMap[output] = value;
    else
        mMap.insert(output, value);
}


/**
 * @brief 根据输出位序号
 * @param output
 * @param name
 * @return
 */
bool PduOutputSort::get(int output, int &value)
{
    QReadLocker locker(mLock);
    bool ret = mMap.contains(output);
    if(ret)
        value = mMap.value(output);

    return ret;
}

/**
 * @brief 输出位数
 * @return
 */
int PduOutputSort::size(void) // 输出位的位数
{
    QReadLocker locker(mLock);
    return mMap.size();
}

