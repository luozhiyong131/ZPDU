#include "pdunetwifi.h"

PduNetWifi::PduNetWifi()
{
    en = false; // 是否开启wifi
    ssid = new PduStrBase();
    pwd = new PduStrBase();
}

PduNetWifi::~PduNetWifi()
{
    delete ssid;
    delete pwd;
}
