#include <QCoreApplication>
#include <QtCore>
#include "pdu/dev/pdudevspied.h"
#include "net/analyze/netdataanalyze.h"

/**
 * @brief 初始化
 */
void initFunction()
{
    /* 创建网络数据解析对象、它分创建UDP接收套接字，广播心跳包，接收数据并进行解析，把数据保存至Hash中 */
    new NetDataAnalyze();
    new PduDevSpied(); // 设备掉线处理
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    initFunction();
//    qDebug() << "AAAAAAAAAA";

    return a.exec();
}
