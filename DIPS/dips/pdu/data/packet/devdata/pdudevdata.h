#ifndef PDUDEVDATA_H
#define PDUDEVDATA_H

#include "pduobjdata.h"
#include "pduenvdata.h"

class PduDevData
{
public:
    PduDevData();
    ~PduDevData();

    PduObjData *line; // 相数据
    PduObjData *loop; // 回路数据
    PduObjData *output; //位数据

    PduEnvData *env; // 环境数据
};

#endif // PDUDEVDATA_H
