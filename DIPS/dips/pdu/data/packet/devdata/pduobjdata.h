#ifndef PDUOBJDATA_H
#define PDUOBJDATA_H
#include "pdudataunit.h"

class PduObjData
{
public:
    PduObjData();
    ~PduObjData();

    PduDataUnit *vol; // 电压
    PduDataUnit *cur; // 电流

    PduDataBase *pow; // 功率
    PduDataBase *ele; // 电能

    PduDataBase *pf; // 功率因素
    PduDataBase *sw; // 开关状态 0 表示未启用

    PduDataBase *carbon; // 排碳量
    PduDataBase *rate; // 电压频率
};

#endif // PDUOBJDATA_H
