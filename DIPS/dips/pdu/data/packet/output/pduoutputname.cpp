/*
 * pduoutputname.cpp
 * PDU设备输出位名称
 *  主要使用map 依据输出位，来保存输出位名称
 *      1、提供，设置输出位接口
 *      2、提供，获取输出位名称接口
 *      3、提供，输出位，位数等接口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduoutputname.h"

PduOutputName::PduOutputName()
{
    mLock = new QReadWriteLock();
}

PduOutputName::~PduOutputName()
{
    delete mLock;
    mMap.clear();
}


/**
 * @brief 设置输出位名称
 * @param output
 * @param name
 */
void PduOutputName::set(int output,const QString &name)
{
    QWriteLocker locker(mLock);
    bool ret = mMap.contains(output);
    if(ret)
    {
        if(mMap.value(output) != name)
            mMap[output] = name;
    }
    else
        mMap.insert(output, name);
}


/**
 * @brief 根据输出位获取名称
 * @param output
 * @param name
 * @return
 */
bool PduOutputName::get(int output, QString &name) //获取输出位名称
{
    QReadLocker locker(mLock);
    bool ret = mMap.contains(output);
    if(ret)
        name = mMap.value(output);

    return ret;
}

QString PduOutputName::get(int output) //获取输出位名称
{
    QString name;
    bool ret = get(output, name);
    if(!ret) name = "Output " + QString::number(output+1);

    return name;
}

/**
 * @brief 输出位数
 * @return
 */
int PduOutputName::size(void) // 输出位的位数
{
    QReadLocker locker(mLock);
    return mMap.size();
}

/**
 * @brief 设置所有输出位名称
 * @param name
 */
void PduOutputName::setAll(const QString &name)
{
    QWriteLocker locker(mLock);
    QMap<int, QString>::const_iterator i;
    for(i=mMap.constBegin(); i!= mMap.constEnd(); ++i)
    {
        int output = i.key();
        mMap[output] = name + QObject::tr(" %1").arg(output); // 自动增加后缀
    }
}



/**
 * @brief 获取输出位列表
 * @param name
 * @return
 */
int PduOutputName::getList(QStringList &name) // 获取输出位列表
{
    QReadLocker locker(mLock);
    QMap<int, QString>::const_iterator i;
    for(i=mMap.constBegin(); i!= mMap.constEnd(); ++i)
        name << i.value();

    return mMap.size();
}
