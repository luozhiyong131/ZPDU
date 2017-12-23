#include "pdudevtype.h"

PduDevType::PduDevType()
{
    ms = -1; /*主从模式 1表示从机、0表示主机 默认为从机*/

    name = new PduStrBase(); //设备名称
    typeStr = new PduStrBase(); //设备型号

    sys = new PduDevSys(); //设备系统信息
    cpu = new PdudevCPU(); //设备CPU信息
}

PduDevType::~PduDevType()
{
    delete name;
    delete typeStr;

    delete sys;
    delete cpu;
}
