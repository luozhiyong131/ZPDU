/*
 * pdu_clearflag.cpp
 *  清除设备标志数据的纪录标志
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdu_clearflag.h"

/**
 * @brief 清除纪录标志
 * @param flag
 * @param size
 */
static void clear_alarm_dataBase(PduDataBase *flag, int size)
{
    for(int i=0; i<size; ++i)
        flag->set(i, 0); // 0表示未发纪录
}

/**
 * @brief 清除数据单元
 * @param unit
 */
static void clear_alarm(PduDataUnit *unit)
{
    int size = unit->alarm->size();
    PduDataBase *flag = unit->flag;
    clear_alarm_dataBase(flag, size); // 清除报警标志

    size = unit->crAlarm->size();
    flag = unit->crFlag;
    clear_alarm_dataBase(flag, size); // 清除临界报警标志
}

/**
 * @brief 清除数据对象
 * @param obj
 */
static void clear_ObjData(PduObjData *obj)
{
    clear_alarm(obj->vol);
    clear_alarm(obj->cur);
}

/**
 * @brief 清除环境数据
 * @param env
 */
static void clear_env(PduEnvData *env)
{
    clear_alarm(env->tem);
    clear_alarm(env->hum);

    clear_alarm_dataBase(env->doorFlag, 2);
    clear_alarm_dataBase(env->waterFlag, 1);
    clear_alarm_dataBase(env->smokeFlag, 1);
}

/**
 * @brief 清除设备数据的纪录标志
 * @param dataPacket
 */
void pdu_da_clearFlag(PduDataPacket *dataPacket)
{
    if(dataPacket->clearFlag == false) // 需要清理
    {
        dataPacket->clearFlag = true; // 已清理
        PduDevData *data = dataPacket->data;

        clear_ObjData(data->line);
        clear_ObjData(data->loop);
        clear_ObjData(data->output);

        clear_env(data->env);
    }
}
