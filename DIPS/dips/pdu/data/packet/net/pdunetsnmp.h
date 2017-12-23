#ifndef PDUNETSNMP_H
#define PDUNETSNMP_H
#include "pdustrbase.h"

class PduNetSNMP
{
public:
    PduNetSNMP();
    ~PduNetSNMP();

    bool en; // V1、V2是否启用
    PduStrBase *get; // GET共同体:
    PduStrBase *set; //SET共同体

    PduStrBase *trap1; //Trap1地址
    PduStrBase *trap2; //Trap2地址

    PduStrBase *server;//SNMP服务器位置
    PduStrBase *node;//SNMP节点

    bool enV3; // SNMP v3
    PduStrBase *usr; // 账号
    PduStrBase *pwd; // 密码:
    PduStrBase *key; // 私钥:
};

#endif // PDUNETSNMP_H
