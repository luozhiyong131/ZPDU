#ifndef PDUHASHMANSLAVE_H
#define PDUHASHMANSLAVE_H
#include "pduhashcom.h"

typedef enum {
    PDU_CMD_DELAY_SW=1, // 设置配制信息
    PDU_CMD_DELAY_TIME,
    PDU_CMD_BUZ_EN,
    PDU_CMD_GROUP_EN,
    PDU_CMD_LCD_EN
}PDU_CMD_DEV_SET_INFO_;



typedef enum {
    PDU_CMD_DEVMAN=8, //设备管理信息 主功能码为8
    PDU_CMD_SETINFO=1, // 设置配制信息
}PDU_CMD_DEV_MANAGE;

void pdu_hashDevSet_save(PduDevManage *manage,pdu_dev_data *data);

#endif // PDUHASHMANSLAVE_H
