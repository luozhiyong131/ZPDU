/*
 * pduusrmanager.cpp
 * PDU设备用户管理类
 *      主要包括：用户表、用户组、输出位权限设置信息
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduusrmanager.h"

PduUsrManager::PduUsrManager()
{
    usrHash = new PduUsrHash(); //用户表
    group = new PduUsrGroup(); // 用户组
    rights = new PduGroupRights(); //输出位权限设置信息
}

PduUsrManager::~PduUsrManager()
{
    delete usrHash;
    delete group;
    delete rights;
}
