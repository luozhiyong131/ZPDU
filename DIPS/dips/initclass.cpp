#include "initclass.h"
#include "pdu/dev/pdudevspied.h"
#include "net/analyze/netdataanalyze.h"


InitClass::InitClass()
{
    initFunction();
}


/**
 * @brief 初始化
 */
void InitClass::initFunction()
{
    /* 创建网络数据解析对象、它分创建UDP接收套接字，广播心跳包，接收数据并进行解析，把数据保存至Hash中 */
    new NetDataAnalyze();
    new PduDevSpied(); // 设备掉线处理
}
