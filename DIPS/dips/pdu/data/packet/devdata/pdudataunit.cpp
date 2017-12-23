/*
 * pdudataunit.cpp
 * PDU数据单元，主要包括
 *  1、最大值、最小值、报警值
 *  2、临界下值，临界上值，临界报警值
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudataunit.h"

PduDataUnit::PduDataUnit()
{
    num = 0;
    value = new PduDataBase();

    min = new PduDataBase(); // 最小值
    max = new PduDataBase(); // 最大值
    alarm = new PduDataBase(); // 报警值
    flag = new PduDataBase(); // 纪录标志位

    crMin = new PduDataBase(); // 临界最小值
    crMax = new PduDataBase(); // 临界最大值
    crAlarm = new PduDataBase(); // 临界报警值
    crFlag = new PduDataBase(); // 纪录标志位
}

PduDataUnit::~PduDataUnit()
{
    delete value;
    delete min; // 最小值
    delete max; // 最大值
    delete alarm; // 报警值
    delete flag;

    delete crMin; // 临界最小值
    delete crMax; // 临界最大值
    delete crAlarm; // 临界报警值
    delete crFlag;
}


