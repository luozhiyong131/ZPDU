/*
 * pduenvdata.cpp
 * PDU数据对象类，主要包括
 *  电压、电流、功率、电能、功率因素、开关状态
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduobjdata.h"

PduObjData::PduObjData()
{
    vol = new PduDataUnit(); // 电压
    cur = new PduDataUnit(); // 电流

    pow = new PduDataBase(); // 功率
    ele = new PduDataBase(); // 电能

    pf = new PduDataBase(); // 功率因素
    sw = new PduDataBase(); // 开关状态

    carbon = new PduDataBase(); // 排碳量
    rate = new PduDataBase(); //电压频率
}


PduObjData::~PduObjData()
{
    delete vol;
    delete cur;

    delete pow;
    delete ele;

    delete pf;
    delete sw;

    delete carbon;
    delete rate;
}
