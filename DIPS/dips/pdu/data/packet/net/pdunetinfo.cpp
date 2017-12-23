#include "pdunetinfo.h"

PduNetInfo::PduNetInfo()
{
    ip = new PduNetIP(); // IP地址
    wifi = new PduNetWifi(); //WIFI信息

    http = new PduNetHttp(); // http
    ssh = new PduNetSSH();

    ftp = new PduNetFTP();
    modbus = new PduNetModbus();
    snmp = new PduNetSNMP();

    telnet = new PduNetTelnet();
    smtp = new PduNetSMTP();
    ntp = new PduNetNTP();

    radius = new PduNetRadius();
}

PduNetInfo::~PduNetInfo()
{
    delete ip;
    delete wifi;

    delete http;
    delete ssh;

    delete ftp;
    delete modbus;
    delete snmp;

    delete telnet;
    delete smtp;
    delete ntp;

    delete radius;
}
