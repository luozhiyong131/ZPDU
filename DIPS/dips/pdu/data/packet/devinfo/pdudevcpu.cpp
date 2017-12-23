#include "pdudevcpu.h"

PdudevCPU::PdudevCPU()
{
    temp = -1; //温度
    use = -1; //使用率

    name = new PduStrBase(); //CPU名称
    frequency = -1; // 处理器频率:

    mem = new PduStrBase(); //内存名称
    memFre = -1; //内存频率
    memSize = -1; //内存大小
}

PdudevCPU::~PdudevCPU()
{
    delete name;
    delete mem;
}
