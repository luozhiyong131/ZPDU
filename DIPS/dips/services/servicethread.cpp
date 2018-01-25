#include "servicethread.h"

ServiceThread::ServiceThread(QObject *parent) : QThread(parent)
{    
    mBuildJson = BuildJson::bulid();
    mWebSocket = new WebSocketClient(this);
    QTimer::singleShot(20*1000,this,SLOT(start()));
    readFile();
}

ServiceThread *ServiceThread::bulid()
{
    static ServiceThread* sington = nullptr;
    if(sington == nullptr)
        sington = new ServiceThread();
    return sington;
}

ServiceThread::~ServiceThread()
{
    isRun = false;
    wait();
}

void ServiceThread::readFile()
{
    mSec = sys_configFile_readInt("timer", WEB_SOCKET);
    if(mSec<=0) {
        mSec = 5;
        sys_configFile_writeParam("timer", QString::number(mSec), WEB_SOCKET);
    }
}

void ServiceThread::readDevList()
{
    QList<int> list;
    pdu_hashData_list(list); // 获取设备类型接口

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
                PduDataPacket *data = devHash->getPacket(devList.at(k)); // 获取设备数据
                if(data->offLine > 0)  // 必须在线才进行检查
                {
                    QJsonObject json;
                    bool ret = mBuildJson->getJson(data, json);
                    if(ret) {
                        ret = mWebSocket->sendMessage(json);
                         mBuildJson->saveJson("test", json);
                         qDebug() << "AAAAAAAAAAA";
                    }
                }

                if(isRun) usleep(1); // 延时1us 让其它线程优先执行
                else return;
            }
        }
    }
}

void ServiceThread::run()
{
    isRun = true;
    while(isRun) {
        readDevList();
        sleep(mSec);
    }
}
