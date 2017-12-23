/*
 * pduoutput.cpp
 * PDU设备输出位相关信息
 *      1、输出位名称
 *      2、输出位排序信息
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduoutput.h"

PduOutput::PduOutput()
{
    name = new PduOutputName();
    sort = new PduOutputSort();
}

PduOutput::~PduOutput()
{
    delete name;
    delete sort;
}
