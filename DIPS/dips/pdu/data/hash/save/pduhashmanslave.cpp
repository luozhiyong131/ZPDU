/*
 * pduhashdevchartsave.cpp
 * PDU设备基本配置信息的保存
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduhashmanslave.h"

/**
 * @brief PDU基本配置功能
 * @param setInfo
 * @param data
 */
static void pdu_hashSet_save(PduDevSetInfo *setInfo,pdu_dev_data *data)
{
    int fc = data->fn[1] & 0x0f;
    switch (fc)
    {
    case PDU_CMD_DELAY_SW: // 延时开关
        if(data->data[0])
            setInfo->enDelay = true;
        else
            setInfo->enDelay = false;
        break;

    case PDU_CMD_DELAY_TIME: // 延时时间
        setInfo->delay = data->data[0] *256 + data->data[1];
        break;

    case PDU_CMD_BUZ_EN: // 蜂鸣器开关
        if(data->data[0])
            setInfo->enBuzzer = true;
        else
            setInfo->enBuzzer = false;
        break;

    case PDU_CMD_GROUP_EN: // 分组开关
        if(data->data[0])
            setInfo->enGroups = true;
        else
            setInfo->enGroups = false;
        break;

    case PDU_CMD_LCD_EN: // LCD常高模式
        if(data->data[0])
            setInfo->enLcd = true;
        else
            setInfo->enLcd = false;
        break;

    default:
        break;
    }
}



/**
 * @brief 设备用户信息保存
 * @param usr
 * @param data
 */
void pdu_hashDevSet_save(PduDevManage *manage,pdu_dev_data *data)
{
    int fc = data->fn[1] >> 4;
    switch (fc)
    {
    case PDU_CMD_SETINFO:
        pdu_hashSet_save(manage->setInfo, data);
        break;

    default:
        qDebug() << "pdu_hashDevSet_save err";
        break;
    }

}




