#include "pdunetipaddr.h"

PduNetIPAddr::PduNetIPAddr()
{
    mode = 0;
    ip = new PduStrBase(); // iP
    gw = new PduStrBase(); // 网关
    mask = new PduStrBase(); // 子网码
    dns = new PduStrBase(); // dns
    dns2 = new PduStrBase(); // dns
}

PduNetIPAddr::~PduNetIPAddr()
{
    delete ip;
    delete gw;
    delete mask;
    delete dns;
    delete dns2;
}

