#include "pdudevaddr.h"

PduDevAddr::PduDevAddr()
{
    area = new PduStrBase(); //区
    group = new PduStrBase(); // 组
    cab = new PduStrBase(); //机柜
}

PduDevAddr::~PduDevAddr()
{
    delete area;
    delete group;
    delete cab;
}
