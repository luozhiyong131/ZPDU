#ifndef PDUNETINFO_H
#define PDUNETINFO_H

#include "pdunetip.h"
#include "pdunetwifi.h"
#include "pdunethttp.h"
#include "pdunetssh.h"
#include "pdunetftp.h"
#include "pdunetmodbus.h"
#include "pdunetsnmp.h"
#include "pdunettelnet.h"
#include "pdunetsmtp.h"
#include "pdunetntp.h"
#include "pdunetradius.h"


class PduNetInfo
{
public:
    PduNetInfo();
    ~PduNetInfo();

    PduNetIP *ip; // IP地址
    PduNetWifi *wifi; //WIFI信息

    PduNetHttp *http; // http
    PduNetSSH *ssh;

    PduNetFTP *ftp;
    PduNetModbus *modbus;
    PduNetSNMP *snmp;

    PduNetTelnet *telnet;
    PduNetSMTP *smtp;
    PduNetNTP *ntp;

    PduNetRadius *radius;
};

#endif // PDUNETINFO_H
