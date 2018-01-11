/*
 * pdudevlist.cpp
 *  PDU设备各类的处理类
 *      1、列出设备种类
 *      2、根据设备代号获取设备名称
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudtname.h"

PduDTName::PduDTName()
{

}

PduDTName *PduDTName::bulid()
{
    static PduDTName* sington = nullptr;
    if(sington == nullptr)
        sington = new PduDTName();
    return sington;
}


/**
 * @brief Hash表中的设备列表
 */
int PduDTName::list(QList<int> &list)
{
    return  pdu_hashData_list(list);
}

/**
 * @brief 定义设备各类列表
 */
int PduDTName::defineList(QList<int> &list)
{
    list.append(PDU_TYPE_IP_PDU);
    list.append(PDU_TYPE_M_PDU);
    list.append(PDU_TYPE_NPM_PDU);
    list.append(PDU_TYPE_RPDU);
    list.append(PDU_TYPE_ZPDU);

    return list.size();
}

bool PduDTName::getName(int type, QString &name)
{
    bool ret = true;

    switch (type)
    {
    case PDU_TYPE_IP_PDU:
         name = "IP-PDU";
        break;

    case PDU_TYPE_M_PDU:
        name = "MPDU";
        break;

    case PDU_TYPE_NPM_PDU:
        name = "NPM";
        break;

    case PDU_TYPE_RPDU:
        name = "RPDU";
        break;

    case PDU_TYPE_ZPDU:
        name = "ZPDU";
        break;

    default:
        qDebug() << "PDU Type err: " << type;
        name = "Lzy";
        ret = false;
        break;
    }

    return ret;
}

QString PduDTName::getName(int type)
{
    QString name;
    bool ret = getName(type, name);
    return name;
}

int PduDTName::getNum(const QString &name)
{
    int num = -1;

    if(name.contains("IP-PDU"))
        num = PDU_TYPE_IP_PDU;
    else if(name.contains("MPDU"))
        num = PDU_TYPE_M_PDU;
    else if(name.contains("NPM"))
        num = PDU_TYPE_NPM_PDU;
    else if(name.contains("RPDU"))
        num = PDU_TYPE_RPDU;
    else if(name.contains("ZPDU"))
        num = PDU_TYPE_ZPDU;

    return num;
}
