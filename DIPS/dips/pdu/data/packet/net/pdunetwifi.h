#ifndef PDUNETWIFI_H
#define PDUNETWIFI_H
#include "pdunetip.h"

class PduNetWifi
{
public:
    PduNetWifi();
    ~PduNetWifi();

    bool en; // 是否开启wifi
    PduStrBase *ssid;
    PduStrBase *pwd;

};

#endif // PDUNETWIFI_H
