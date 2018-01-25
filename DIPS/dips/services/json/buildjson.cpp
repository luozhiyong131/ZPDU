/*
 * buildjson.cpp
 * 根据设备数据包生成Josn格式
 *
 *  Created on: 2018年1月11日
 *      Author: Lzy
 */
#include "buildjson.h"

BuildJson::BuildJson()
{

}

BuildJson *BuildJson::bulid()
{
    static BuildJson* sington = nullptr;
    if(sington == nullptr)
        sington = new BuildJson();
    return sington;
}



bool BuildJson::getJson(int devType, QString &ip, int num, QJsonObject &json)
{
    bool ret = false;
    PduDataPacket *packet = pdu_hd_get(devType, ip, num);
    if(packet) {
        ret = getJson(packet, json);
    }

    return ret;
}

bool BuildJson::getJson(PduDataPacket *packet, QJsonObject &json)
{
    bool ret = true;

    if(packet->offLine > 0) {
        head(packet, json);
        pduDevData(packet, json);
        pduNetData(packet->net, json);
    } else {
        ret = false;
    }

    return ret;
}

bool BuildJson::saveJson(const QString &name, QJsonObject &json)
{
    QJsonDocument jsonDoc(json);
    QByteArray ba = jsonDoc.toJson();
    QFile file(name+".json");
    bool ret = file.open(QIODevice::WriteOnly);
    if(ret) {
        file.write(ba);
        file.close();
    } else {
        qDebug() << "write json file failed";
    }

    return ret;
}


void BuildJson::head(PduDataPacket *packet, QJsonObject &json)
{
    QJsonObject obj;
    obj.insert("ip", packet->ip->get());
    obj.insert("dev_num", packet->devNum); //

    QString devType = PduDTName::bulid()->getName(packet->devType);
    obj.insert("dev_type", devType);
    obj.insert("dev_spec", packet->devSpec);

    QString name = packet->info->type->name->get();
    obj.insert("dev_name", name);

    json.insert("head_info", QJsonValue(obj));
}


void BuildJson::pduDataUnit(PduDataUnit *unit, int id, const QString &str, double rate, QJsonObject &obj)
{
    int value = unit->value->get(id) ;
    if(value >= 0) obj.insert(str+"_value", value / rate);

    value = unit->min->get(id);
    if(value >= 0) obj.insert(str+"_min", value / rate);

    value = unit->max->get(id);
    if(value >= 0) obj.insert(str+"_max", value / rate);

    value = unit->alarm->get(id);
    if(value >= 0) obj.insert(str+"_alarm", value / rate);

    value = unit->crMin->get(id);
    if(value >= 0) obj.insert(str+"_critical_min", value / rate);

    value = unit->crMax->get(id);
    if(value >= 0) obj.insert(str+"_critical_max", value / rate);

    value = unit->crAlarm->get(id);
    if(value >= 0) obj.insert(str+"_critical_alarm", value / rate);
}

void BuildJson::pduObjData(PduObjData *ObjData, int id, QJsonObject &obj)
{
    pduDataUnit(ObjData->vol, id, "vol", COM_RATE_VOL, obj);
    pduDataUnit(ObjData->cur, id, "cur", COM_RATE_CUR, obj);

    int value = ObjData->pow->get(id);
    if(value >= 0) obj.insert("pow", value / COM_RATE_POW);

    value = ObjData->ele->get(id);
    if(value >= 0) obj.insert("ele", value / COM_RATE_ELE);

    value = ObjData->pf->get(id);
    if(value >= 0) obj.insert("PF", value / COM_RATE_PF);

    value = ObjData->sw->get(id);
    if(value >= 0) obj.insert("switch", value);
}

void BuildJson::pduLineData(PduObjData *ObjData, QJsonObject &json, int mode)
{
    QJsonArray jsonArray;
    QString modeStr = "line";
    if(mode) modeStr = "loop";

    int num = ObjData->vol->value->size();
    for(int i=0; i<num; ++i)
    {
        QJsonObject subObj;
        subObj.insert("id", i+1);
        subObj.insert("name", modeStr +" "+ QString::number(i+1));

        pduObjData(ObjData, i, subObj);
        jsonArray.append(subObj);
    }

    if(num > 0) json.insert(QString("%1_item_list").arg(modeStr), QJsonValue(jsonArray));
}

void BuildJson::pduOutputData(PduObjData *ObjData, PduOutput *output, QJsonObject &json)
{
    QJsonArray jsonArray;

    int num = ObjData->cur->value->size();
    for(int i=0; i<num; ++i)
    {
        QJsonObject subObj;
        subObj.insert("id", i+1);
        QString name = output->name->get(i);
        subObj.insert("name", name);

        pduObjData(ObjData, i, subObj);
        jsonArray.append(subObj);
    }

    if(num > 0) json.insert("output_item_list", QJsonValue(jsonArray));
}

void BuildJson::pduThData(PduDataUnit *unit, QJsonObject &json, int mode)
{
    QJsonArray jsonArray;
    QString modeStr = "tem";
    if(mode) modeStr = "hum";

    int num = unit->value->size();
    for(int i=0; i<num; ++i)
    {
        QJsonObject subObj;
        subObj.insert("id", i+1);
        subObj.insert("name", modeStr + " " + QString::number(i+1));

        pduDataUnit(unit, i, modeStr, COM_RATE_TEM, subObj);
        jsonArray.append(subObj);
    }

    if(num > 0) json.insert(QString("%1_item_list").arg(modeStr), QJsonValue(jsonArray));
}

void BuildJson::pduEnvItemData(PduDataBase *data, QJsonObject &json, int mode)
{
    QJsonArray jsonArray;

    QString modeStr = "door";
    switch (mode) {
    case 2: modeStr = "door"; break;
    case 3: modeStr = "water"; break;
    case 4: modeStr = "smoke"; break;
    default: break;
    }

    int num = data->size();
    for(int i=0; i<num; ++i)
    {
        QJsonObject subObj;
        subObj.insert("id", i+1);
        subObj.insert("name", modeStr + " " + QString::number(i+1));

        int value = data->get(i);
        if(value >= 0) subObj.insert("value", value);
        jsonArray.append(subObj);
    }

    if(num > 0) json.insert(QString("%1_list").arg(modeStr), QJsonValue(jsonArray));
}

void BuildJson::pduEnvData(PduEnvData *env, QJsonObject &obj)
{
    int fn = 0;
    QJsonObject json;

    pduThData(env->tem, json, fn++);
    pduThData(env->hum, json, fn++);
    pduEnvItemData(env->door, json, fn++);
    pduEnvItemData(env->water, json, fn++);
    pduEnvItemData(env->smoke, json, fn++);

    for(int i=0; i<2; ++i)
        obj.insert("segment"+QString::number(i+1), "");
    obj.insert("env_info", QJsonValue(json));
}

void BuildJson::pduDevData(PduDataPacket *packet, QJsonObject &obj)
{   
    obj.insert("company", "CLEVER");
    obj.insert("version", 1);

    PduDevData *devData = packet->data;
    pduLineData(devData->line, obj, 0);
    pduLineData(devData->loop, obj, 1);
    pduOutputData(devData->output, packet->output, obj);
    pduEnvData(devData->env, obj);

    for(int i=0; i<6; ++i)
        obj.insert("segment"+QString::number(i+1), "");
}


void BuildJson::pduIpData(PduNetIPAddr *net, QJsonObject &json)
{
    QJsonObject obj;

    obj.insert("mode", net->mode);
    obj.insert("ip", net->ip->get());
    obj.insert("mask", net->mask->get());
    obj.insert("gw", net->gw->get());
    obj.insert("dns", net->dns->get());
    json.insert("ip_addr", QJsonValue(obj));
}


void BuildJson::pduNetData(PduNetInfo *net, QJsonObject &obj)
{
    pduIpData(net->ip->ip, obj);
}
