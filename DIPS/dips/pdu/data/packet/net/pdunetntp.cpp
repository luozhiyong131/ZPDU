#include "pdunetntp.h"

PduNetNTP::PduNetNTP()
{
    en = true;
    server = new PduStrBase(); // NTP服务器:
    port = 123; //端口
    timeZone = new PduStrBase(); //时区
}

PduNetNTP::~PduNetNTP()
{
    delete server;
    delete timeZone;
}
