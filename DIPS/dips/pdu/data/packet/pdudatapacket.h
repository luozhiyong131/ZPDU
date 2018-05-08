#ifndef PDUDATAPACKET_H
#define PDUDATAPACKET_H

#include "devinfo/pdudevinfo.h"
#include "devdata/pdudevdata.h"
#include "output/pduoutput.h"
#include "usr/pduusrmanager.h"
#include "net/pdunetinfo.h"
#include "devmanage/pdudevmanage.h"
#include "chart/pdudevchart.h"

#define PDU_OFF_LINE_TIME   10 // PDU离线时间

class PduDataPacket
{
public:
    PduDataPacket();
    ~PduDataPacket();

    volatile int state; // 工作状态 ==0 正常
    volatile int offLine; //离线标志 > 0在线

    volatile int devNum;  // 设备号
    volatile int devType; //设备类型
    volatile int devSpec; // 设备规格 A\B\C\D
    volatile int txType; // 通讯类型 0  1:SNMP  2：Zebra
    PduStrBase *ip; //设备IP

    PduDevInfo *info; // 设备型号、设备地址
    PduDevData *data; //设备数据
    PduOutput *output; //输出位信息

    PduUsrManager *usr; //设备用户
    PduNetInfo *net; //设备网络信息

    PduDevManage *manage; //设备管理信息
    PduDevChart *chart; //设备图表信息

    bool clearFlag; // 为真，表示已清理，为false 表示需要清理
    int sqlId;
};

#endif // PDUDATAPACKET_H
