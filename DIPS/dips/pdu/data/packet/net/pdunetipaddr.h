#ifndef PDUNETIPADDR_H
#define PDUNETIPADDR_H
#include "pdustrbase.h"

class PduNetIPAddr
{
public:
    PduNetIPAddr();
    ~PduNetIPAddr();

    char mode; /* 网络模式 0手动设置 1 自动获取*/
    PduStrBase *ip; // iP
    PduStrBase *gw; // 网关
    PduStrBase *mask; // 子网码
    PduStrBase *dns; // dns
    PduStrBase *dns2; // 备用dns
};

#endif // PDUNETIPADDR_H
