#ifndef PDUHASHDEVCHARTSAVE_H
#define PDUHASHDEVCHARTSAVE_H
#include "pduhashcom.h"

typedef enum {
    PDU_CMD_CHART_POW=1, // 功率数据
    PDU_CMD_CHART_CUR,
    PDU_CMD_CHART_VOL,

    PDU_CMD_CHART_TEMP,
    PDU_CMD_CHART_HUM
}PDU_CMD_DEV_CHART_;

typedef enum {
    PDU_CMD_DEVCHART=9, //设备图表信息 主功能码为9
}PDU_CMD_DEV_CHART;

void pdu_hashDevChart_save(PduDevChart *chart,pdu_dev_data *data);

#endif // PDUHASHDEVCHARTSAVE_H
