#ifndef PDUDEVINFO_H
#define PDUDEVINFO_H
#include "pdudevtype.h"
#include "pdudevaddr.h"

class PduDevInfo
{
public:
    PduDevInfo();
    ~PduDevInfo();

    PduDevType *type; //设备型号
    PduDevAddr *addr; //设备地址
};

#endif // PDUDEVINFO_H
