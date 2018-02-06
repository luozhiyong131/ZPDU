/*
 * 配置文件公共类
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "configbase.h"

ConfigBase::ConfigBase()
{
    initFun();
}

void ConfigBase::initFun()
{
    mUploadMode = readfile("UploadMode", 1);
    mDownMode = readfile("DownMode", 1);
    mSetCmd = readfile("SetCmd", 0);
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

