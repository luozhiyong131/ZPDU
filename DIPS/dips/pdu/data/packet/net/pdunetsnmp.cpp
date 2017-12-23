#include "pdunetsnmp.h"

PduNetSNMP::PduNetSNMP()
{
    en = true; // V1、V2是否启用
    get = new PduStrBase(); // GET共同体:
    set = new PduStrBase(); //SET共同体

    trap1 = new PduStrBase(); //Trap1地址
    trap2 = new PduStrBase(); //Trap2地址

    server = new PduStrBase();//SNMP服务器位置
    node = new PduStrBase();//SNMP节点

    enV3 = false; // SNMP v3
    usr = new PduStrBase(); // 账号
    pwd = new PduStrBase(); // 密码:
    key = new PduStrBase(); // 私钥:
}

PduNetSNMP::~PduNetSNMP()
{
    delete get;
    delete set;

    delete trap1;
    delete trap2;

    delete server;
    delete node;

    delete usr;
    delete pwd;
    delete key;
}
