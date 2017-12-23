/*
 * pduhashdevchartsave.cpp
 * PDU设备图表数据的保存
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduhashdevchartsave.h"

/**
 * @brief 图表类，包括时间，和数据。
 *          目前只保存过去24小时的数据，不保存时间， 时间预留，
 * @param chart
 * @param data
 */
static void pdu_hashChart_save(PduChartInfo *chart,pdu_dev_data *data)
{
    PduDataBase *ptr = chart->data;
    int sizeBit = 2;
    if(ptr)
        pdu_saveHash_intData(ptr, data->len, data->data, sizeBit);
}


/**
 * @brief 温湿度图表数据
 * @param th
 * @param data
 */
static void pdu_hashTH_save(PduChartth *th,pdu_dev_data *data)
{
    int fc = data->fn[1]&0x0f;
    switch (fc)
    {
    case PDU_CMD_1:
        pdu_hashChart_save(th->C1, data);
        break;

    case PDU_CMD_2:
        pdu_hashChart_save(th->C2, data);
        break;

    case PDU_CMD_3:
        pdu_hashChart_save(th->C3, data);
        break;

    case PDU_CMD_4:
        pdu_hashChart_save(th->C4, data);
        break;
    }
}

/**
 * @brief 设备用户信息保存
 * @param usr
 * @param data
 */
void pdu_hashDevChart_save(PduDevChart *chart,pdu_dev_data *data)
{
    int fc = data->fn[1] >> 4;
    switch (fc)
    {
    case PDU_CMD_CHART_POW: // 功率数据
        pdu_hashChart_save(chart->pow, data);
        break;

    case PDU_CMD_CHART_CUR: // 电流数据
        pdu_hashChart_save(chart->cur, data);
        break;

    case PDU_CMD_CHART_VOL: // 电压数据
        pdu_hashChart_save(chart->vol, data);
        break;

    case PDU_CMD_CHART_TEMP: // 温度
        pdu_hashTH_save(chart->tem, data);
        break;

    case PDU_CMD_CHART_HUM: // 湿度
        pdu_hashTH_save(chart->hum, data);
        break;

    default:
        qDebug() << "pdu_hashDevChart_save err";
        break;
    }
}


