/*
 * pdudevdata.cpp
 * PDU数据类，主要包括
 *  1、相数据
 *  2、输出位数据
 *  3、环境数据
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudevdata.h"

PduDevData::PduDevData()
{
    line = new PduObjData(); // 相数据
    loop = new PduObjData();
    output = new PduObjData(); //位数据

    env = new PduEnvData(); // 环境数据
}

PduDevData::~PduDevData()
{
    delete line;
    delete loop;
    delete output;

    delete env;
}
