/*
 * pdudevmanage.cpp
 * PDU设备配置信息
 *  主要包括：设备设置信息、设备分组信息、定时开关、超限断电、开关控制
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudevmanage.h"

PduDevManage::PduDevManage()
{
    setInfo = new PduDevSetInfo(); //设备设置信息
    groupInfo = new PduGroupsInfo(); //设备分组信息

    timeSW = new PduTimeSwitch(); //定时开关
    cutOff = new PduCutOff(); //超限断电

    sw = new PduSWControl(); //开关控制
}


PduDevManage::~PduDevManage()
{
    delete setInfo;
    delete groupInfo;
    delete timeSW;
    delete cutOff;
    delete sw;
}
