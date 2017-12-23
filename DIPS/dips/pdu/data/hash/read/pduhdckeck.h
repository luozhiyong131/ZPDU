#ifndef PDUHDCKECK_H
#define PDUHDCKECK_H
#include "pduhashread.h"

typedef enum {
    PDU_DATA_ALARM_OK=0, // 数据正常
    PDU_DATA_ALARM_MIN, // 数据过小
    PDU_DATA_ALARM_MAX, // 数据过大

    PDU_DATA_ALARM_CRMIN, // 数据小于临界下限
    PDU_DATA_ALARM_CRMAX, // 数据大于临界上限
}PPDU_DATA_ALARM_NO;

int pdu_data_checkAlarm(PduDataUnit *dataUnit, int bit);
int pdu_data_checkCrAlarm(PduDataUnit *dataUnit, int bit);
int pdu_data_alarmCheck(PduDataUnit *dataUnit, int bit);
#endif // PDUHDCKECK_H
