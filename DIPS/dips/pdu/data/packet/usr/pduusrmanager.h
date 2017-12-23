#ifndef PDUUSRMANAGER_H
#define PDUUSRMANAGER_H
#include "pduusrhash.h"
#include "pdugrouprights.h"

class PduUsrManager
{
public:
    PduUsrManager();
    ~PduUsrManager();

    PduUsrHash *usrHash; //用户表
    PduUsrGroup *group; // 用户组
    PduGroupRights *rights; //输出位权限设置信息
};

#endif // PDUUSRMANAGER_H
