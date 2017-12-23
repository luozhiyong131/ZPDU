/*
 * pduenvdata.cpp
 * PDU环境数据类，主要包括
 *  1、温湿度、门禁、水浸、烟雾等
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduenvdata.h"

PduEnvData::PduEnvData()
{
    tem = new PduDataUnit(); // 温度
    hum = new PduDataUnit(); // 湿度

    door = new PduDataBase(); // 门禁
    doorFlag = new PduDataBase();

    water = new PduDataBase(); // 水浸
    waterFlag = new PduDataBase();

    smoke = new PduDataBase(); // 烟雾
    smokeFlag = new PduDataBase();
}


PduEnvData::~PduEnvData()
{
    delete tem;
    delete hum;

    delete door;
    delete doorFlag;

    delete water;
    delete waterFlag;

    delete smoke;
    delete smokeFlag;
}
