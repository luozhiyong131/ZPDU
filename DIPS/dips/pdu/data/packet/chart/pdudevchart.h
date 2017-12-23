#ifndef PDUDEVCHART_H
#define PDUDEVCHART_H
#include "pduchartth.h"

class PduDevChart
{
public:
    PduDevChart();
    ~PduDevChart();

    PduChartInfo *pow; //功率
    PduChartInfo *cur; //电流
    PduChartInfo *vol; //电压

    PduChartth *tem; //温度
    PduChartth *hum; //湿度
};

#endif // PDUDEVCHART_H
