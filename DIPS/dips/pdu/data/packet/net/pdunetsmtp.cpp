#include "pdunetsmtp.h"

PduNetSMTP::PduNetSMTP()
{
    usr = new PduStrBase(); // SMTP账号
    pwd = new PduStrBase(); // 密码

    server = new PduStrBase();// SMTP服务器:
    port = 25; // 端口

    mode = new PduStrBase(); // 认证方式
    test = new PduStrBase(); // SMTP设置测试 接收账号
}

PduNetSMTP::~PduNetSMTP()
{
    delete usr;
    delete pwd;
    delete server;
    delete mode;
    delete test;
}
