#ifndef PDUDEVCPU_H
#define PDUDEVCPU_H
#include "pdustrbase.h"

class PdudevCPU
{
public:
    PdudevCPU();
    ~PdudevCPU();

    int temp; //温度
    int use; //使用率

    PduStrBase *name; //CPU名称
    int frequency; // 处理器频率:

    PduStrBase *mem; //内存名称
    int memFre; //内存频率
    int memSize; //内存大小
};

#endif // PDUDEVCPU_H
