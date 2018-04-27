/*
 * 配置文件公共类
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "configbase.h"
#include "pdu/dev/devSetting/devsetting.h"

ConfigBase::ConfigBase()
{
    initFun();
}

void ConfigBase::initFun()
{
    mUploadMode = readfile("UploadMode", 1);
    mDownMode = readfile("DownMode", 1);
    mSetCmd = readfile("SetCmd", 0);
    if(mSetCmd) { //设置通道
        net_dev_data pkt;
        pkt.addr = 0;
        pkt.fn[0] = 20; //系统维护
        pkt.fn[1] = 0x04; //通道切换
        uchar data[2] ={1, 1};
        data[0] = mUploadMode; //上传通道 1 2
        data[1] = mDownMode; //下发通道 1 2 3
        pkt.data = data;
        pkt.len = 2;  //[2字节]
        DevSetting::bulid()->sentData(pkt);

        cmdSetClear(); //置零
    }
}

int ConfigBase::readfile(const QString &str, int deValue)
{
    QString prefix = getPrefix();
    QString string = QString("%1_%2").arg(prefix).arg(str);
    int value =  sys_configFile_readInt(string, prefix);
    if(value < 0) {
        value = deValue;
        sys_configFile_writeParam(string, QString::number(value), prefix);
    }
    return value;
}

void ConfigBase::cmdSetClear()
{
    QString prefix = getPrefix();
    QString string = QString("%1_SetCmd").arg(prefix);
    int value = mSetCmd = 0;
    sys_configFile_writeParam(string, QString::number(value), prefix);
}

ConfigBase *ConfigBase::bulid()
{
    static ConfigBase* sington = nullptr;
    if(sington == nullptr)
        sington = new ConfigBase();
    return sington;
}

