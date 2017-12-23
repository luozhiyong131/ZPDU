#ifndef PDUENVDATA_H
#define PDUENVDATA_H
#include "pdudataunit.h"

class PduEnvData
{
public:
    PduEnvData();
    ~PduEnvData();

    PduDataUnit *tem; // 温度
    PduDataUnit *hum; // 湿度

    PduDataBase *door; // 门禁
    PduDataBase *doorFlag;

    PduDataBase *water; // 水浸
    PduDataBase *waterFlag;

    PduDataBase *smoke; // 烟雾
    PduDataBase *smokeFlag;
};

#endif // PDUENVDATA_H
