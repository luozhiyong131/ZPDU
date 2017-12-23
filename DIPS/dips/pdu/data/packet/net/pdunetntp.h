#ifndef PDUNETNTP_H
#define PDUNETNTP_H
#include "pdustrbase.h"

class PduNetNTP
{
public:
    PduNetNTP();
    ~PduNetNTP();

    bool en;
    PduStrBase *server; // NTP服务器:
    int port; //端口
    PduStrBase *timeZone; //时区
};

#endif // PDUNETNTP_H
