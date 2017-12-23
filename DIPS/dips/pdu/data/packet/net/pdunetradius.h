#ifndef PDUNETRADIUS_H
#define PDUNETRADIUS_H
#include "pdustrbase.h"

class PduNetRadius
{
public:
    PduNetRadius();
    ~PduNetRadius();

    bool basicSet; // 基本设置
    bool radius; // Radius认证
    bool local; // RADIUS服务器无响应时启用本地认证

    PduStrBase *serve; // 认证服务器:
    PduStrBase *key; // 公钥
    int auPort; // 认证端口
    int serPort; // 账号端口
};

#endif // PDUNETRADIUS_H
