/*
 * 配置文件公共基类
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "configbase.h"

ConfigBase::ConfigBase()
{

}


/**
 * @brief 获取串口名称
 * @return 串口名
 */
QString ConfigBase::getSerialName()
{
    QString prefix = getPrefix();
    QString str = QString("%1_COM").arg(prefix);
    return sys_configFile_readStr(str, prefix);
}

/**
 * @brief 设置串口名
 * @param name
 */
void ConfigBase::setSerialName(const QString &name)
{
    QString prefix = getPrefix();
    QString str = QString("%1_COM").arg(prefix);
    sys_configFile_writeParam(str, name, prefix);
}

/**
 * @brief 获取相数
 * @return
 */
QString ConfigBase::getBaudRate()
{
    QString prefix = getPrefix();
    QString str = QString("%1_baud_rate").arg(prefix);
    str = sys_configFile_readStr(str, prefix);
    if(str.isEmpty())  str = "9600";
    return str;
}

/**
 * @brief 设置相数
 * @param num
 */
void ConfigBase::setBaudRate(const QString &rate)
{
    QString prefix = getPrefix();
    QString str = QString("%1_baud_rate").arg(prefix);
    sys_configFile_writeParam(str, rate, prefix);
}

