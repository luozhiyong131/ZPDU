/*
 * pdu_alarmdev.cpp
 *  设备报警线程处理
 *   1、设备报警时，设备的信息会保存至gAlarmDevHash表
 *   2、报警处理，在线程中处理 每隔三秒会执行一次报警处理
 *   3、根据报警设备的ip 和设备号，调用PDU_AlarmRecord类接口来纪录报警信息
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdu_alarmdev.h"

static QReadWriteLock *gLock = new QReadWriteLock();
static alarmDevHash *gAlarmDevHash = new alarmDevHash(); // 所有的离线设备，都会保存至此Hash表中

/**
 * @brief 增加 报警设备
 * @param ip 报警IP
 * @param num 设备号
 */
void pdu_alarmDev_append(const QString &ip, int num)
{
    QWriteLocker locker(gLock);
    gAlarmDevHash->insert(ip, num);
}

PDU_AlarmDev::PDU_AlarmDev(QObject *parent) : QThread(parent)
{
    isRun = isOpen = false;
    mLock = gLock;
    mHash = gAlarmDevHash;

    mAlarmRecord = new PDU_AlarmRecord(this);
    QTimer::singleShot(6*1000,this,SLOT(initFunction()));
}

PDU_AlarmDev::~PDU_AlarmDev()
{
    isRun = false;
    wait();
}

/**
 * @brief 延时启动线程
 */
void PDU_AlarmDev::initFunction(void)
{
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    mTimer->start(6*1000);
}

/**
 * @brief 定时启动线程
 */
void PDU_AlarmDev::timeoutDone(void)
{
    if(isRun == false) {
        isRun = true;
        start();
    }
}

/**
 * @brief 设备IP列表
 * @return
 */
int PDU_AlarmDev::listIP(QStringList &ip)
{
    int num = mHash->size();
    if(num>0)
    {
        //        QReadLocker locker(mLock);
        alarmDevHash::const_iterator i;
        for(i=mHash->constBegin(); i!=mHash->constEnd(); ++i)
        {
            if(!ip.contains(i.key()))
                ip << i.key();
        }
    }

    return num;
}

/**
 * @brief 设备列表
 * @return
 */
int PDU_AlarmDev::listDev(QString &ip,QList<int> &devList)
{
    //    QReadLocker locker(mLock);
    devList = mHash->values(ip);
    qSort(devList);

    return devList.size();
}

/**
 * @brief 设备报警
 * @param list
 */
void PDU_AlarmDev::alarmDev(void)
{
    QStringList ipList;
    listIP(ipList);

    for(int i=0; i<ipList.size(); ++i)
    {
        QList<int> devList;
        listDev(ipList[i], devList);

        for(int j=0; j<devList.size(); ++j) {
            mAlarmRecord->setDev(ipList[i], devList[j]);
            usleep(5);
        }
    }

    QWriteLocker locker(gLock);
    mHash->clear();
}

/**
 * @brief 报警设备检查
 * @return
 */
void PDU_AlarmDev::alarmCheck(void)
{
    int num = mHash->size();
    if(num > 0)
        alarmDev();

}

/**
 * @brief 线程入口函数
 */
void PDU_AlarmDev::run(void)
{    
    alarmCheck();
    isRun = false;
}

alarmDevHash *getAlarmHash()
{
    return gAlarmDevHash;
}
