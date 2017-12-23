/*
 * pduhashoutputsave.cpp
 * PDU设备输出位信息的保存
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduhashoutputsave.h"

/**
 * @brief 设置输出位的名称
 * @param name
 * @param data
 */
void pdu_output_name(PduOutputName *name, pdu_dev_data *data)
{
    int output = data->fn[1];
    if((output >= 0) && (output < 32))    // 输出位最大32位
    {
        QString str;
        bool ret = char_to_string(str,data->data, data->len);
        if(ret)
            name->set(output, str); // 设置输出位的名称
    }
    else
        qDebug() << "pdu_output_name err";
}

/**
 * @brief 设置输出位序号
 * @param dev
 * @param data
 */
void pdu_output_sort(PduOutputSort *dev, pdu_dev_data *data)
{
    int output = data->fn[1];
    if((output >= 0) && (output < 32))    // 输出位最大32位
    {
        int sort =  data->data[0];
        if((sort >= 0) && (sort < 32))
            dev->set(output, sort);
        else
            qDebug() << "pdu_output_sort err";
    }
    else
        qDebug() << "pdu_output err";
}


/**
 * @brief 设置输出位分组信息
 * @param dev
 * @param data
 */
void pdu_output_group(PduGroupsInfo *groupInfo, pdu_dev_data *data)
{
    int output = data->fn[1];
    if((output >= 0) && (output < 64))     // 输出位最大32位
    {
        QString str;
        bool ret = char_to_string(str,data->data, data->len);
        if(ret)
        {
            if(output >= 0)
                groupInfo->set(output, str); // 设置输出位的名称
        }
    }
    else if(output == -1)
        groupInfo->del();
    else
        qDebug() << "pdu_output_group err";
}


/**
 * @brief 输出位超限断电
 * @param dev
 * @param data
 */
void pdu_output_cutOff(PduCutOff *cut, pdu_dev_data *data)
{
    int output = data->fn[1];
    if((output >= 0) && (output < 64))    // 输出位最大32位
    {
        bool ret = false;
        if(data->data[0])
            ret = true;
        cut->set(output,ret);
    }
    else if(output == -1)
        cut->del();
    else
        qDebug() << "pdu_output_cutOff err";
}

/**
 * @brief 组管理， 超限断电
 * @param cut
 * @param data
 */
void pdu_group_cutOff(PduCutOff *cut, pdu_dev_data *data)
{
    bool en = false;
    if(data->fn[1])
        en = true;

    QString str;
    bool ret = char_to_string(str,data->data, data->len);
    if(ret)
        cut->setGroup(str,en);
}




/**
 * @brief 输出位开关控制
 * @param dev
 * @param data
 */
void pdu_output_SWctrl(PduSWControl *ctrl, pdu_dev_data *data)
{
    int output = data->fn[1];
    if((output >= 0) && (output < 64))    // 输出位最大32位
    {
        bool ret = false;
        if(data->data[0])
            ret = true;
        ctrl->set(output,ret);
    }
    else if(output == -1)
        ctrl->del();
    else
        qDebug() << "pdu_output_cutOff err";
}


/**
 * @brief 组管理， 控制开关
 * @param cut
 * @param data
 */
void pdu_group_SWctrl(PduSWControl *ctrl, pdu_dev_data *data)
{
    bool en = false;
    if(data->fn[1])
        en = true;

    QString str;
    bool ret = char_to_string(str,data->data, data->len);
    if(ret)
        ctrl->setGroup(str,en);
}

/**
 * @brief 输出位全开、全断
 * @param ctrl
 * @param data
 */
void pdu_all_SWctrl(PduSWControl *ctrl, pdu_dev_data *data)
{
    bool en = false;
    if(data->fn[1])
        en = true;
    ctrl->setSW(en);
}


/**
 * @brief 输出位定时开关
 * @param dev
 * @param data
 */
void pdu_output_time(PduTimeSwitch *timeSW, pdu_dev_data *data)
{
    int output = data->fn[1];
    if((output >= 0) && (output < 64))    // 输出位最大32位
    {
        QString str;
        bool ret = char_to_string(str,data->data, data->len);
        if(ret)
            timeSW->set(output,str);
    }
    else if(output == -1)
        timeSW->del();
    else
        qDebug() << "pdu_output_cutOff err";
}


/**
 * @brief 组管理， 定时开关
 * @param cut
 * @param data
 */
void pdu_group_time(PduTimeSwitch *timeSW, pdu_dev_data *data)
{
    QString str;
    bool ret = char_to_string(str,data->data, data->len);
    if(ret)
    {
        QStringList strlist = str.split("; ",QString::SkipEmptyParts);
        if(strlist.size() == 2)
            timeSW->setGroup(strlist.at(0),strlist.at(1));
    }
}


