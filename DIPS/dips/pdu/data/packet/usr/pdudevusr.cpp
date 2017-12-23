/*
 * pdudevusr.cpp
 * PDU设备用户信息
 *  主要包括
 *      用户名、密码、邮件、手机、所属组
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudevusr.h"

PduDevUsr::PduDevUsr()
{
    usr = new PduStrBase();
    pwd = new PduStrBase();

    emil = new PduStrBase();
    emil2 = new PduStrBase();
    emil3 = new PduStrBase();

    phone = new PduStrBase();
    group = new PduStrBase();
}


PduDevUsr::~PduDevUsr()
{
    delete usr;
    delete pwd;

    delete emil;
    delete emil2;
    delete emil3;

    delete phone;
    delete group;
}

