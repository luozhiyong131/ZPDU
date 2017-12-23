#include "pdunetradius.h"

PduNetRadius::PduNetRadius()
{
    basicSet = false ; // 基本设置
    radius = false; // Radius认证
    local = false; // RADIUS服务器无响应时启用本地认证

    serve = new PduStrBase(); // 认证服务器:
    key = new PduStrBase(); // 公钥

    auPort = 1812; // 认证端口
    serPort = 1813; // 账号端口
}

PduNetRadius::~PduNetRadius()
{
    delete serve;
    delete key;
}
