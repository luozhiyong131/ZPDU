/*
 * pduchartinfo.cpp
 * PDU设置图表信息
 *  1、时间
 *  2、数据
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduchartinfo.h"

PduChartInfo::PduChartInfo()
{
    data = new PduDataBase();
    time = new PduStrBase();
}

PduChartInfo::~PduChartInfo()
{
    delete data;
    delete time;
}
