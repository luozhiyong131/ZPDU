#include "pduhdckeck.h"


/**
 * @brief 检查报警状态
 */
int pdu_data_checkAlarm(PduDataUnit *dataUnit, int bit)
{
    int ret = PDU_DATA_ALARM_OK;

    if(dataUnit->alarm->get(bit) > 0)
    {
        int data = dataUnit->value->get(bit);
        if(data >= 0)
        {
            int min = dataUnit->min->get(bit);
            if(min >= 0) {
                int max = dataUnit->max->get(bit);
                if(max >= 0) {
                    if(min <= max)  {
                        if(data < min) {
                            ret = PDU_DATA_ALARM_MIN; // 值过小
                        }
                        if(data > max)
                            ret = PDU_DATA_ALARM_MAX; // 值过大
                    } else
                        ret = -4; // 阈值错误
                } else
                    ret = -3; // 最大值无效
            } else
                ret = -2; // 最小值无效
        }  else
            ret = -1; // 当前值无效值
    }

    return ret;
}

/**
 * @brief 检查临界状态
 * @return
 */
int pdu_data_checkCrAlarm(PduDataUnit *dataUnit, int bit)
{
    int ret = PDU_DATA_ALARM_OK;

    int data = dataUnit->value->get(bit);
    if(data >= 0)
    {
        int min = dataUnit->crMin->get(bit);
        if(min >= 0) {
            int max = dataUnit->crMax->get(bit);
            if(max >= 0) {
                if(min <= max)  {
                    if(data < min)
                        ret = PDU_DATA_ALARM_CRMIN; // 值过小
                    if(data > max)
                        ret = PDU_DATA_ALARM_CRMAX; // 值过大
                } else
                    ret = -4; // 阈值错误
            } else
                ret = -3; // 最大值无效
        } else
            ret = -2; // 最小值无效
    }  else
        ret = -1; // 当前值无效值

    return ret;
}

/**
 * @brief 先检查报警状态，再检查临界状态
 * @return
 */
int pdu_data_alarmCheck(PduDataUnit *dataUnit, int bit)
{
    int ret = pdu_data_checkAlarm(dataUnit, bit);
    if(ret == PDU_DATA_ALARM_OK)
        ret = pdu_data_checkCrAlarm(dataUnit, bit);

    return ret;
}
