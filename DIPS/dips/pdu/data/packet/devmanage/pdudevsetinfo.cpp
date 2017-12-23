/*
 * pdudevsetinfo.cpp
 * 对应网页，设备管理中的设备设置页面
 *  主要包括
 *      1、工作模式
 *      2、延时信息
 *      3、模式设置等
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudevsetinfo.h"

PduDevSetInfo::PduDevSetInfo()
{
    enDelay = false;
    delay = -1;

    enBuzzer = true;
    enGroups = false;
    enLcd = false;
}

