#include "pdunetip.h"

PduNetIP::PduNetIP()
{
    ip = new PduNetIPAddr();
    ip_v6 = new PduNetIPAddr();
}

PduNetIP::~PduNetIP()
{
    delete ip;
    delete ip_v6;
}



