#ifndef PDUDEVUSR_H
#define PDUDEVUSR_H
#include "pdustrbase.h"

class PduDevUsr
{
public:
    PduDevUsr();
    ~PduDevUsr();

    PduStrBase *usr; // 用户
    PduStrBase *pwd; // 密码

    PduStrBase *emil; // 邮件
    PduStrBase *emil2; // 邮件
    PduStrBase *emil3; // 邮件

    PduStrBase *phone; // 电话
    PduStrBase *group; // 用户组
};

#endif // PDUDEVUSR_H
