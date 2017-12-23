/*
 * pdudevchart.cpp
 * PDU设备的图表信息类
 *      主要包括：功率、电流、电压、湿度、湿度等图表信息
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudevchart.h"

PduDevChart::PduDevChart()
{
    pow = new PduChartInfo(); //功率
    cur = new PduChartInfo(); //电流
    vol = new PduChartInfo(); //电压

    tem = new PduChartth(); //温度
    hum = new PduChartth(); //湿度
}

PduDevChart::~PduDevChart()
{
    delete pow;
    delete cur;
    delete vol;
    delete tem;
    delete hum;
}

