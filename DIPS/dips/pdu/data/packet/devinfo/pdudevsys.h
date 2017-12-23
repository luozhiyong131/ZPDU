#ifndef PDUDEVSYS_H
#define PDUDEVSYS_H
#include "pdustrbase.h"


class PduDevSys
{
public:
    PduDevSys();
    ~PduDevSys();

    PduStrBase *version; //系统版本
    PduStrBase *upTime; //系统最后更新时间
    int runTime; //系统运行时
};

#endif // PDUDEVSYS_H
