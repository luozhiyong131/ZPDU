#ifndef PDUNETSMTP_H
#define PDUNETSMTP_H
#include "pdustrbase.h"

class PduNetSMTP
{
public:
    PduNetSMTP();
    ~PduNetSMTP();

    PduStrBase *usr; // SMTP账号
    PduStrBase *pwd; // 密码

    PduStrBase *server;// SMTP服务器:
    int port; // 端口

    PduStrBase *mode; // 认证方式
    PduStrBase *test; // SMTP设置测试 接收账号
};

#endif // PDUNETSMTP_H
