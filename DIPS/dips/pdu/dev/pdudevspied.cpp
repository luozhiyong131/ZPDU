/*
 * pdudevspied.cpp
 * PDU设备监视，对其是否离线、是否报警等进行监视、并统计设备总数
 *      1、设备在线总线，保存在 int gPduDevNum 变量中
 *      2、设备报警数量,
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudevspied.h"
#include "pduhashdata.h"

static pdu_devNumStr *gPduDevNum = new pdu_devNumStr();

pdu_devNumStr *get_pdu_num(void)
{
    return gPduDevNum;
}

PduDevSpied::PduDevSpied(QObject *parent) : QThread(parent)
{
    isRun = false;
    memset(gPduDevNum, 0, sizeof(pdu_devNumStr));

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    mTimer->start(2300);

    QTimer::singleShot(23*1000,this,SLOT(initFunction()));
}

PduDevSpied::~PduDevSpied()
{
    wait();
    delete mTimer;
}

void PduDevSpied::initFunction(void)
{
//    mDLSave = new PduDLSaveThread(this); // 保存线程
    mOffLine = new PDU_OFFLineDev(this); // 离线线程
    mAlarmDev = new PDU_AlarmDev(this); // 报警线程处理

//    DbSlaveRecordsThread::get(this); ////==== 设备纪录
//    DbRtPduSaveThread::get(this);
}

/**
 * @brief 定时器启动线程
 */
void PduDevSpied::timeoutDone()
{
    if(isRun == false) {
        start();
    }
}


/**
 * @brief 设备状态检查
 */
void PduDevSpied::checkDevState(void)
{
    int num = 0, alarmNum=0, lineNum=0, offNum=0;
    QList<int> list;
    pdu_hashData_list(list); // 获取设备类型接口

    static int count = 0; count++;
    for(int i=0; i<list.size(); ++i)
    {
        PduHashIP *hashIP =  pdu_hashData_getHash(list.at(i)); // 获取某种设备类型的对象
        QStringList ipList;
        hashIP->list(ipList); // 根据设备类型，获取设备IP列表

        for(int j=0; j<ipList.size(); ++j)
        {
            PduDevHash *devHash = hashIP->getDev(ipList.at(j));// 获取设备对象
            QList<int> devList;
            devHash->list(devList); // 根据设备IP，获取设备地址列表 级联

            for(int k=0; k<devList.size(); ++k)
            {
                num++; // 设备数量
                PduDataPacket *data = devHash->getPacket(devList.at(k)); // 获取设备数据
                if(data->offLine > 0)  // 必须在线才进行检查
                {
                    data->offLine -= 1; // 标志位减少
                    if(data->offLine>0)
                    {
                        lineNum++; // 在线设备数量加一
                        if(data->state) // 设备不正常
                        {
                            alarmNum++; // 报警数量增加1
                            // qDebug() << "dev alarm" << list.at(i) << ipList.at(j) << devList.at(k);
                            pdu_alarmDev_append(ipList.at(j), devList.at(k)); // 设备报警处理
                            data->clearFlag = false; // 需要清楚标志位
                        }
                        else // 设备工作正常
                        {
                            pdu_da_clearFlag(data);//清除设备报警纪录标志位
                        }
                    }
                    else /*if(data->offLine == 0)*/ // 设备掉线
                    {
                        // 掉线设备信息 设备类型、设备IP、设备号
                        // qDebug() << "dev off line" << list.at(i) << ipList.at(j) << devList.at(k);
                        off_line_append(ipList.at(j), devList.at(k));  // 设备掉线处理
                    }
                }
                else
                {
                    offNum++;
                }

                usleep(1); // 延时1us 让其它线程优先执行
//                if(count%3 == 0) SignalDispatch::get()->dispatch(data); // 每2抄钏更新一次机房拓朴图
            }
        }
    }

    gPduDevNum->num = num;
    gPduDevNum->line = lineNum;
    gPduDevNum->alarm = alarmNum;
    gPduDevNum->off = offNum;

    qDebug() << "Num:" << num
             << "   lineNum:" << lineNum
             << "   OffNum:" << offNum;
}


/**
 * @brief 线程入口函数
 */
void PduDevSpied::run(void)
{
    if(isRun == false)
    {
        isRun = true;
        checkDevState();

        isRun = false;
    }
}

