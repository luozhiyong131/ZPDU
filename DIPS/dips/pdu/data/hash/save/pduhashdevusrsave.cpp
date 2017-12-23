/*
 * pduhashdevusrsave.cpp
 * PDU设备用户信息的保存
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduhashdevusrsave.h"

#define STRING_SPLIT "; " //字符串分割符


/**
 * @brief 设备用户名、密码
 * @param usrHash
 * @param data
 */
static void pdu_usrName_save(PduUsrHash *usrHash, pdu_dev_data *data)
{
    QString str;
    bool ret = char_to_string(str, data->data, data->len);
    if(ret)
    {
        QStringList strlist = str.split(STRING_SPLIT,QString::SkipEmptyParts);
        if(strlist.size() == 2)
            usrHash->setPwd(strlist.at(0), strlist.at(1));// 0用户名，1
        else
            qDebug() << "pdu_usrName_save err" << strlist;
    }
}

/**
 * @brief pdu_usrDel_save
 * @param usrHash
 * @param data
 */
static void pdu_usrDel_save(PduUsrHash *usrHash, pdu_dev_data *data)
{
    QString str;
    bool ret = char_to_string(str, data->data, data->len);
    if(ret)
        usrHash->del(str);
}

/**
 * @brief 设置用户的邮件地址
 * @param usrHash
 * @param data
 */
static void pdu_usrEmail_save(PduUsrHash *usrHash, pdu_dev_data *data)
{
    QString str;
    bool ret = char_to_string(str, data->data, data->len);
    if(ret)
    {
        QStringList strlist = str.split(STRING_SPLIT,QString::SkipEmptyParts);
        if(strlist.size() == 3)
        {
            QString strId = strlist.at(1);
            bool ok;
            int id = strId.toInt(&ok);
            if(ok)
                usrHash->setEmil(strlist.at(0), id, strlist.at(2));
            else
                qDebug() << "pdu_usrEmail_save err" << strlist;
        }
        else
            qDebug() << "pdu_usrEmail_save err" << strlist;

    }
}


/**
 * @brief 用户手机
 * @param usrHash
 * @param data
 */
static void pdu_usrPhone_save(PduUsrHash *usrHash, pdu_dev_data *data)
{
    QString str;
    bool ret = char_to_string(str, data->data, data->len);
    if(ret)
    {
        QStringList strlist = str.split(STRING_SPLIT,QString::SkipEmptyParts);
        if(strlist.size() == 2)
            usrHash->setPhone(strlist.at(0),  strlist.at(1));
        else
            qDebug() << "pdu_usrPhone_save err" << strlist;
    }
}

/**
 * @brief 用户所属组
 * @param usrHash
 * @param data
 */
static void pdu_usrGroup_save(PduUsrHash *usrHash, pdu_dev_data *data)
{
    QString str;
    bool ret = char_to_string(str, data->data, data->len);
    if(ret)
    {
        QStringList strlist = str.split(STRING_SPLIT,QString::SkipEmptyParts);
        if(strlist.size() == 2)
            usrHash->setGroup(strlist.at(0),  strlist.at(1));
        else
            qDebug() << "pdu_usrGroup_save err" << strlist;
    }
}

/**
 * @brief PDU设备用户信息
 * @param usrHash
 * @param data
 */
static void pdu_hashUsr_save(PduUsrHash *usrHash,pdu_dev_data *data)
{
    int fc = data->fn[1] & 0x0f;
    switch (fc)
    {
    case PDU_CMD_USR_NAME: // 设备用户名、密码
        pdu_usrName_save(usrHash, data);
        break;

    case PDU_CMD_USR_EMAIL: // 用户邮件
        pdu_usrEmail_save(usrHash, data);
        break;

    case PDU_CMD_USR_PHONE: //用户手机
        pdu_usrPhone_save(usrHash, data);
        break;

    case PDU_CMD_USR_GROUP: //用户组
        pdu_usrGroup_save(usrHash, data);
        break;

    case PDU_CMD_USR_DEL:
        pdu_usrDel_save(usrHash, data);
        break;

    case PDU_CMD_USR_ClEAR:
        usrHash->del();
        break;
    }
}



/**
 * @brief 用户组权限设置
 * @param group
 * @param data
 */
static void pdu_hashGroup_save(PduUsrGroup *group,pdu_dev_data *data)
{
    int fc = data->fn[1]& 0x0f;
    switch (fc)
    {
    // ==== 用户组权限设置，以后增加

    default:
        break;
    }
}


/**
 * @brief 输出位权限
 * @param group
 * @param data
 */
static void pdu_hashOutput_save(PduGroupRights *rights,pdu_dev_data *data)
{
    int fc = data->fn[1]& 0x0f;
    switch (fc)
    {
    // ==== 输出位权限设置，以后增加

    default:
        break;
    }
}


/**
 * @brief 设备用户信息保存
 * @param usr
 * @param data
 */
void pdu_hashDevUsr_save(PduUsrManager *usr,pdu_dev_data *data)
{
    int fc = data->fn[1] >> 4;
    switch (fc)
    {
    case PDU_CMD_USR_INFO: //用户信息
        pdu_hashUsr_save(usr->usrHash, data);
        break;

    case PDU_CMD_USRGROUPP:
        pdu_hashGroup_save(usr->group, data);
        break;

    case PDU_CMD_USROUTPUT:
        pdu_hashOutput_save(usr->rights, data);
        break;

    default:
        qDebug() << "pdu_hashDevUsr_save err";
        break;
    }

}
