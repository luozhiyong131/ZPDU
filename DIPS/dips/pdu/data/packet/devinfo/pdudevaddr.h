#ifndef PDUDEVADDR_H
#define PDUDEVADDR_H
#include "pdustrbase.h"

class PduDevAddr
{
public:
    PduDevAddr();
    ~PduDevAddr();

    PduStrBase *area; //区
    PduStrBase *group; // 组
    PduStrBase *cab; //机柜
};

#endif // PDUDEVADDR_H
