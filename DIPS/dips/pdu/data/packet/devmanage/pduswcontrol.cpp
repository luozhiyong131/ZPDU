/*
 * pduswcontrol.cpp
 * PDU设备开关控制功能
 *      1、继续自PduCutOff
 *      2、增加总开关状态
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduswcontrol.h"

PduSWControl::PduSWControl()
{
    sw = true;
}

void PduSWControl::setSW(bool en)
{
    sw = en;
    setAll(en);
}

bool PduSWControl::getSW(void)
{
    return sw;
}

