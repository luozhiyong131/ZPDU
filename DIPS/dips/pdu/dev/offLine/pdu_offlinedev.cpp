/*
 * pdu_offlinedev.cpp
 * PDU离线设备处理
 *    1、设备离线时，设备信息会保存至gOffLineHash表中
 *    2、PDU_OFFLineDev类，启动线程，把离线设备保存至数据库中
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdu_offlinedev.h"

static QReadWriteLock *gLock = new QReadWriteLock();
static offLineHash *gOffLineHash = new offLineHash(); // 所有的离线设备，都会保存至此Hash表中
extern bool gSlaveing;

/**
 * @brief 增加 离线设备
 * @param ip 离线IP
 * @param num 设备号
 */
void off_line_append(const QString &ip, int num)
{
    QWriteLocker locker(gLock);
    gOffLineHash->insert(ip, num);
}

PDU_OFFLineDev::PDU_OFFLineDev(QObject *parent) : QThread(parent)
{
    isRun = isOpen = false;
    mLock = gLock;
    mHash = gOffLineHash;

    QTimer::singleShot(6*1000,this,SLOT(initFunction()));
    connect(this, SIGNAL(offLineSig(QString, QString, QString)),
            this, SLOT(offLineDevSlot(QString, QString, QString)));
}

PDU_OFFLineDev::~PDU_OFFLineDev()
{
    isRun = false;
    wait();
}

void PDU_OFFLineDev::initFunction(void)
{
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    mTimer->start(6*1000);
}

/**
 * @brief 定时启动线程
 */
void PDU_OFFLineDev::timeoutDone(void)
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
int PDU_OFFLineDev::listIP(QStringList &ip)
{
    int num = mHash->size();
    if(num>0)
    {
        //        QReadLocker locker(mLock);
        offLineHash::const_iterator i;
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
int PDU_OFFLineDev::listDev(QString &ip,QList<int> &devList)
{
    //    QReadLocker locker(mLock);
    devList = mHash->values(ip);
    qSort(devList);

    return devList.size();
}

/**
 * @brief 设备离线响应函数
 * @param ip 设备IP
 * @param name 设备名
 * @param str 提示信息
 */
void PDU_OFFLineDev::offLineDevSlot(QString ip, QString name, QString str)
{
    if(isOpen == false)
    {
        isOpen = true;

        QString msg = tr("设备离线");
        msg += "\n";
        msg += tr("IP:%1  设备名:%2").arg(ip).arg(name);
        msg += "\n";
        msg += str;

        // InfoMsgBox box(0, msg); // 提示

        isOpen = false;
    }
}

void PDU_OFFLineDev::offLineCloudLog(PduDataPacket *dataPacket)
{
    QString room = dataPacket->info->addr->area->get();
    QString cab = dataPacket->info->addr->cab->get();
    QString name = dataPacket->info->type->name->get();
    if(!cab.isEmpty()) {
        QString str = tr("检测到机房:%1 机柜:%2 中的PDU设备离线，").arg(room).arg(cab);
        int num = dataPacket->devNum;
        QString slaveStr, ip = dataPacket->ip->get();
        if(num) slaveStr = tr(" 副机%1 ").arg(num);
        QString logMsg = str + tr("设备IP：%1，%2 设备名：%3，").arg(ip).arg(slaveStr).arg(name);

#if defined(CLOUD_MODULE)
        CloudOffLineItem item;
        item.room = room;
        item.cab = cab;
        item.devType = dataPacket->devType;
        item.ip = ip;
        item.num = num;
        item.msg = logMsg;

        while(gSlaveing) usleep(1); gSlaveing = true;
        CloudOffLine::bulid()->saveLog(item);
        gSlaveing = false;
#endif
    }
}


/**
 * @brief 增加离线日志
 * @param ip 设备IP
 * @param num 设备号
 */
void PDU_OFFLineDev::offLineDevLog(QString &ip, int num)
{
    PduDataPacket *dataPacket = pdu_hd_getPacket(ip, num);
    if(dataPacket)
    {

        QString name = dataPacket->info->type->name->get();

        sOffLineLog log;
        log.devIP = ip;
        log.devName = name;
        log.title = tr("设备离线");

        QString str = tr("检测到此设备离线");
        if(num)
            str = tr("检测到副机%1离线").arg(num);
        log.msg = str;
//        sql_insert_offLine(log);

        QString smsMsg = tr("检测到PDU设备离线，设备名：%1，设备IP：%2").arg(name).arg(ip);
//        sim_sent_sms(smsMsg); //发送短信

        QString title = tr("PDU设备离线报警");
//        email_sent_message(title, smsMsg);

        if(isOpen == false)
            emit offLineSig(ip, name, str);
        offLineCloudLog(dataPacket);
    }

}

/**
 * @brief 设备离线函数
 * @param list
 */
void PDU_OFFLineDev::offLineDev(void)
{
    QStringList ipList;
    listIP(ipList);
    for(int i=0; i<ipList.size(); ++i)
    {
        /**
         * @brief 如果只有一台设备离线，侧保存此设备日志信息，
         *          如果主机离线，侧只保存主机离线信息
         *          如果多台副机离线，侧保存副机信息
         */
        QList<int> devList;
        int rtn = listDev(ipList[i], devList);
        if(rtn == 1)
            offLineDevLog(ipList[i], devList[0]);
        else
        {
            bool ret = devList.contains(0);
            if(ret)
                offLineDevLog(ipList[i], 0); // 保存主机设备信息
            else
            {
                for(int j=0; j<devList.size(); ++j)
                    offLineDevLog(ipList[i], devList[j]);
            }
        }
        usleep(5);
    }

    QWriteLocker locker(gLock);
    mHash->clear();
}

/**
 * @brief 离线设备检查
 * @return
 */
void PDU_OFFLineDev::offLineCheck(void)
{
    int num = mHash->size();
    if(num > 0)
        offLineDev();
}

/**
 * @brief 线程入口函数
 */
void PDU_OFFLineDev::run(void)
{
    offLineCheck();
    isRun = false;
}
