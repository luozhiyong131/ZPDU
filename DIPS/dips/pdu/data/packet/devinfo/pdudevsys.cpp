#include "pdudevsys.h"

PduDevSys::PduDevSys()
{
    version = new PduStrBase(); //系统版本
    upTime = new PduStrBase(); //系统最后更新时间
    runTime = -1; //系统运行时
}

PduDevSys::~PduDevSys()
{
    delete version;
    delete upTime;
}
