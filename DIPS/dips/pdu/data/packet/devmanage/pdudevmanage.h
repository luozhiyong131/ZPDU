#ifndef PDUDEVMANAGE_H
#define PDUDEVMANAGE_H
#include "pdudevsetinfo.h"
#include "pdugroupsinfo.h"
#include "pdutimeswitch.h"
#include "pducutoff.h"
#include "pduswcontrol.h"

class PduDevManage
{
public:
    PduDevManage();
    ~PduDevManage();

    PduDevSetInfo *setInfo; //设备设置信息
    PduGroupsInfo *groupInfo; //设备分组信息

    PduTimeSwitch *timeSW; //定时开关
    PduCutOff *cutOff; //超限断电

    PduSWControl *sw; //开关控制
};

#endif // PDUDEVMANAGE_H
