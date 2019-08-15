#include "zebra_jointhread.h"

Zebra_JoinThread::Zebra_JoinThread(QObject *parent) : QThread(parent)
{
    mZebraData = Zebra_DataPacket::bulid();
    start();
}

Zebra_JoinThread::~Zebra_JoinThread()
{
    isRun = false;
    wait();
}


void Zebra_JoinThread::delay(int s)
{
    for(int i=0; i<s; ++i) {
        if(isRun) sleep(1);
    }
}

void Zebra_JoinThread::workDown()
{
    QStringList joins = mZebraData->getJoinList();
    Zebra_Im *im = Zebra_Im::bulid();

    for(int i=0; i<joins.size(); ++i)
    {
        QString id = joins.at(i);
        int ret = im->joinChannel(id.toStdString(), "zpdu");
        if(ret < 0) {
            mZebraData->removeJoin(id);
            qDebug() << " Zebra_JoinThread err" << id << ret;
        }
        delay(1); if(isRun) break;
    }
}

void Zebra_JoinThread::run()
{
    isRun = true;

    while (isRun)
    {
        delay(5);
        workDown();
    }
}
