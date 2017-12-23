#ifndef PDUNETIP_H
#define PDUNETIP_H
#include "pdunetipaddr.h"

class PduNetIP
{
public:
    PduNetIP();
    ~PduNetIP();

    PduNetIPAddr *ip;
    PduNetIPAddr *ip_v6;

};

#endif // PDUNETIP_H
