#include "pdudevinfo.h"

PduDevInfo::PduDevInfo()
{
    type = new PduDevType(); //设备型号
    addr = new PduDevAddr(); //设备地址
}


PduDevInfo::~PduDevInfo()
{
    delete type;
    delete addr;
}
