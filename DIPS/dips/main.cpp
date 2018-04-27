#include <QCoreApplication>
#include "pdu/dev/pdudevspied.h"
#include "net/analyze/netdataanalyze.h"
#include "servicethread.h"
#include "zebra/common_debug.h"
#include "zebra/zebra_client.h"
#include "autoaddfriend/auto_get_zebra_server_ip.h"
#include "web/web.h"


void initFunction()
{
    /* 创建网络数据解析对象、它分创建UDP接收套接字，广播心跳包，接收数据并进行解析，把数据保存至Hash中 */
    new NetDataAnalyze();
    new PduDevSpied(); // 设备掉线处理
    new ServiceThread();
#ifdef ZEBRA_MODULE
    MY_DEBUG(1, LIGHT_PURPLE) << "hello zebra" << DEBUG_END;
    zebra_client::get_instance()->init();
    AutoGetServer::get_instance();
#endif

#ifdef WEB_MODULE
    qDebug() << "[Web]_ hello web";
    WEB::getInstance();
#endif
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    initFunction();

    return a.exec();
}
