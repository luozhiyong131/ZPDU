#ifndef PDUDEVTYPE_H
#define PDUDEVTYPE_H
#include "pdudevsys.h"
#include "pdudevcpu.h"

class PduDevType
{
public:
    PduDevType();
    ~PduDevType();

    char ms; /*主从模式 1表示从机、0表示主机 默认为从机*/
    PduStrBase *name; //设备名称
    PduStrBase *typeStr; //设备型号

    PduDevSys *sys; //设备系统信息
    PdudevCPU *cpu; //设备CPU信息
};

#endif // PDUDEVTYPE_H
