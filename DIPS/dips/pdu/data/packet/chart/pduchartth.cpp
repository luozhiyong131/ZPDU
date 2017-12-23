/*
 * pduchartinfo.cpp
 * PDU设备的温湿度图表信息
 *      每个设备默认是4个温湿度
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduchartth.h"

PduChartth::PduChartth()
{    
    C1 = new PduChartInfo();
    C2 = new PduChartInfo();
    C3 = new PduChartInfo();
    C4 = new PduChartInfo();
}

PduChartth::~PduChartth()
{
    delete C1;
    delete C2;
    delete C3;
    delete C4;
}

