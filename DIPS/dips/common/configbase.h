#ifndef CONFIGBASE_H
#define CONFIGBASE_H
#include "sysconfigfile.h"

enum {
    PDU_Upload_DisEnable=0, // PDU上传数据失效
    PDU_Upload_TCP=1,   // 上传数据使用TCP通道
    PDU_Upload_Zebra=2, // 上传数据使用Zebra通道

    PDU_Down_DisEnable=0, // 下发数据失效
    PDU_Down_TCP=1,  // 下发命令使用Tcp通道
    PDU_Down_Zebra=2,  // 下发命令使用Zebra通道
    PDU_Down_TcpZebra = 3,  // 下发命令使用双因子验证功能

    PDU_CMD_Set = 1,  // 修改传输模式
    PDU_CMD_NoSet = 0,
};

class ConfigBase
{
    ConfigBase();
public:
    static ConfigBase *bulid();

    int getUploadMode() { return mUploadMode; }
    int getDownMode() { return mDownMode; }

    int getCmdSet(){ return mSetCmd; }
    void cmdSetClear();

protected:
    void initFun();
    int readfile(const QString &str, int deValue);
    QString getPrefix(){return "trans_mode";}

private:
    int mUploadMode;
    int mDownMode;
    int mSetCmd;
};

#endif // CONFIGBASE_H
