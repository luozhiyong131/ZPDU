/*
 * pdu_alarmrecord.cpp
 *  设备报警处理
 *    当设备报警时，会调用setDev() 此接口
 *      1、首先查检输入相相关报警信息
 *      2、再次检查输出位相关报警信息
 *      3、最后检查环境报警信息
 *
 *   每一个报警状态信息，都会产生一条日志纪录
 *   但一个设备只发一次邮件，一条短信
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdu_alarmrecord.h"

extern bool gSlaveing;

PDU_AlarmRecord::PDU_AlarmRecord(QObject *parent) : QThread(parent)
{
    isOpen = false;
    connect(this, SIGNAL(alarmSig(QString)),this, SLOT(alarmSlot(QString))); // 报警提示框
}


void PDU_AlarmRecord::alarmCloudLog(s_AlarmLog &log)
{
    QString room = mDataPacket->info->addr->area->get();
    QString cab = mDataPacket->info->addr->cab->get();
    QString name = mDataPacket->info->type->name->get();
    if(!cab.isEmpty()) {
        QString str = tr("检测到机房:%1 机柜:%2 中的PDU设备报警，").arg(room).arg(cab);
        int num = mDataPacket->devNum;
        QString slaveStr, ip = mDataPacket->ip->get();
        if(num) slaveStr = tr(" 副机%1 ").arg(num);
        QString logMsg = str + tr("设备IP：%1，%2 设备名：%3，报警内容:%4 %5")
                .arg(ip).arg(slaveStr).arg(name).arg(log.line).arg(log.msg);

#if defined(CLOUD_MODULE)
        CloudAlarmItem item;
        item.room = room;
        item.cab = cab;
        item.devType = mDataPacket->devType;
        item.ip = ip;
        item.num = num;
        item.alarmType = log.type;
        item.msg = logMsg;

        while(gSlaveing) usleep(1); gSlaveing = true;
        CloudAlarmLog::bulid()->saveLog(item);
        gSlaveing = false;
#endif
    }
}

void PDU_AlarmRecord::insertAlarm(s_AlarmLog &log)
{
//    sql_insert_alarm(log); // 写入数据库中
}

/**
 * @brief 报警数据单元
 * @param unit 单元数据
 * @param log  日志信息
 * @param str 单位
 * @return true 有报警信息
 */
bool PDU_AlarmRecord::alarmDataUnit(PduDataUnit *unit, s_AlarmLog &log, const QString &str)
{
    //    if(!is_vol)
    //        log.line = log.line +tr(%1);

    bool ret = false;
    QString logLine = log.line;
    QString logMsg = log.msg;

    int size = unit->alarm->size();
    for(int i=0; i<size; ++i)
    {
        int value =  unit->alarm->get(i);
        if(value > 0) // 已经报警
        {
            value = unit->flag->get(i);
            if(value != 1) // 需要纪录
            {
                int j=i+1;
                log.line = logLine + tr("%1").arg(j); // 填写输出位，及相数

                value = unit->value->get(i);
                QString currentCirInfoStr = QString(tr("  最小值：%1%2   最大值：%3%4").arg(unit->min->get(i)/10.0).arg(str).arg(unit->max->get(i)/10.0)).arg(str);
                log.msg = logMsg + tr(" 当前值：%1").arg(value/10.0) + str + currentCirInfoStr;
                insertAlarm(log); // 写入数据库中
                alarmCloudLog(log);
                smsMsg += " \n" + log.type + "; " + log.line +"; " + log.msg +"; \n"; //邮件、短信信息
                unit->flag->set(i, 1);
                ret = true;
            }
        }
    }

    return ret;
}

/**
 * @brief 报警数据对象
 * @param obj 数据对象
 * @param log 日志
 * @return true 有报警产生
 */
bool PDU_AlarmRecord::alarmDataObject(PduObjData *obj, s_AlarmLog &log)
{
    QString logType = log.type;
    QString lonLine = log.line;  //保存初值

    log.type = logType + tr("电压报警");
    bool retVol = alarmDataUnit(obj->vol, log, "V");

    log.type = logType + tr("电流报警");
    log.line = lonLine; //在电压报警中已经修改了log.line，需要初始化
    bool retCur = alarmDataUnit(obj->cur, log, "A");

    return (retVol || retCur);
}

/**
 * @brief 初始化报警日志结构体
 * @param log
 */
void PDU_AlarmRecord::initAlarmLog(s_AlarmLog &log)
{
    QString name;
    int num = mDataPacket->devNum;
    if(num)
        name = tr("副机:%1").arg(num);

    name += mDataPacket->info->type->name->get();
    log.devIP = mDataPacket->ip->get();
    log.devName = name;
    log.title = tr("设备报警");
    log.type.clear();
    log.line.clear();
    log.msg.clear();
}

/**
 * @brief 输入相报警
 * @param data 输入相数据
 * @return true
 */
bool PDU_AlarmRecord::alarmLineData(PduDevData *data)
{
    s_AlarmLog log;
    initAlarmLog(log);

    log.type = tr("相");
    log.line = tr("相数:L");

    return alarmDataObject(data->line,log);
}

bool PDU_AlarmRecord::alarmLoopData(PduDevData *data)
{
    s_AlarmLog log;
    initAlarmLog(log);

    log.type = tr("回路");
    log.line =tr("C");

    return alarmDataObject(data->loop,log);
}

/**
 * @brief 输出位报警
 * @param data 输出位数据
 * @return true
 */
bool PDU_AlarmRecord::alarmBitData(PduDevData *data)
{
    s_AlarmLog log;
    initAlarmLog(log);

    log.type = tr("输出位");
    log.line =tr("输出位:");

    return alarmDataObject(data->output,log);
}

/**
 * @brief 温湿度报警
 * @param envData 环境数据
 * @param log
 * @return true
 */
bool PDU_AlarmRecord::alarmEnvTH(PduEnvData *envData, s_AlarmLog &log)
{
    log.type = tr("温度报警");
    log.line = tr("温度:");
    bool retTem = alarmDataUnit(envData->tem, log, "°C");

    log.type = tr("湿度报警");
    log.line = tr("湿度:");
    bool retHum = alarmDataUnit(envData->hum, log, "%");

    return (retTem || retHum);
}

/**
 * @brief 门禁报警
 * @param envData 环境数据
 * @param log
 * @return
 */
bool PDU_AlarmRecord::alarmDoor(PduEnvData *envData, s_AlarmLog &log)
{
    bool ret = false;
    log.type = tr("门禁报警");

    int size = envData->door->size();
    for(int i=0; i<size; ++i)
    {
        int value = envData->door->get(i);
        if(value == 2)
        {
            value = envData->doorFlag->get(i);
            if(value != 1)
            {
                log.line = tr("门%1").arg(i+1);
                log.msg = log.line + tr("打开");
                insertAlarm(log); // 写入数据库中
                smsMsg += " \n" + log.type + "; " + log.msg +"; \n";

                envData->doorFlag->set(i, 1);
                ret = true;
            }
        }
    }

    return ret;
}

/**
 * @brief 水浸、烟雾报警记录
 * @param alarm 报警数据
 * @param flag 纪录标志位
 * @param log
 * @return true
 */
bool PDU_AlarmRecord::alarmEnvDataBase(PduDataBase *alarm, PduDataBase *flag, s_AlarmLog &log)
{
    bool ret = false;

    int size = alarm->size();
    for(int i=0; i<size; ++i)
    {
        int value = alarm->get(i);
        if(value == 2)
        {
            value = flag->get(i);
            if(value != 1)
            {
                log.msg = log.line + tr("报警");
                insertAlarm(log); // 写入数据库中
                smsMsg += " \n" + log.type + "; " + log.msg +"; \n";

                flag->set(i, 1);
                ret = true;
            }
        }
    }

    return ret;
}

/**
 * @brief 环境报警数据
 * @param envData 环境数据
 * @return true
 */
bool PDU_AlarmRecord::alarmEnv(PduEnvData *envData)
{
    s_AlarmLog log;
    initAlarmLog(log);
    bool retTH = alarmEnvTH(envData, log);

    initAlarmLog(log);
    bool retDoor = alarmDoor(envData, log);

    initAlarmLog(log);
    log.type = tr("水浸报警");
    bool retWater = alarmEnvDataBase(envData->water, envData->waterFlag, log);

    initAlarmLog(log);
    log.type = tr("烟雾报警");
    bool retSmoke = alarmEnvDataBase(envData->smoke, envData->smokeFlag, log);

    return (retTH || retDoor || retWater || retSmoke);
}

/**
 * @brief 报警提示对话框
 * @param str
 */
void PDU_AlarmRecord::alarmSlot(QString str)
{
    if(isOpen == false)
    {
        isOpen = true;
//        InfoMsgBox box(0,str); // 提示
        isOpen = false;
    }
}

/**
 * @brief 发送邮件、短信
 */
void PDU_AlarmRecord::sentEmail(void)
{
    QString salve;
    int num = mDataPacket->devNum;
    if(num)
        salve = tr("副机:%1").arg(num);

    QString name = mDataPacket->info->type->name->get();
    QString ip = mDataPacket->ip->get();

    QString str = tr("检测到PDU设备报警，%1 设备名：%2，设备IP：%3，\n").arg(salve).arg(name).arg(ip);
    str += smsMsg;
//    sim_sent_sms(str); //发送短信

    QString title = tr("PDU设备异常报警");
//    email_sent_message(title, str);

    emit alarmSig(str);
    smsMsg.clear();
}

/**
 * @brief 报警数据入口函数
 * @param devData 设备数据
 */
void PDU_AlarmRecord::alarmDev(PduDevData *devData)
{
    bool retLine = alarmLineData(devData);
    bool retLoop = alarmLoopData(devData);
    bool retBit = alarmBitData(devData);
    bool retEnv = alarmEnv(devData->env);

    if(retLine || retLoop || retBit || retEnv)
        sentEmail();
}

/**
 * @brief 设置设备
 * @param ip 设备IP
 * @param num 设备号
 */
void PDU_AlarmRecord::setDev(QString &ip, int num)
{
    PduDataPacket *dataPacket = pdu_hd_getPacket(ip, num);
    if(dataPacket)
    {
        mDataPacket = dataPacket;
        alarmDev(dataPacket->data);
    }
}





