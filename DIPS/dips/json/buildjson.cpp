#include "buildjson.h"
#include "pdu/dev/devType/pdudtname.h"
#include "common.h"

BuildJson::BuildJson()
{

}

bool BuildJson::getJson(int devType, QString &ip, int num)
{
    bool ret = false;
    PduDataPacket *packet = pdu_hd_get(devType, ip, num);
    if(packet) {
        if(packet->offLine > 0) {

        }
    }


    return ret;
}


bool BuildJson::head(PduDataPacket *packet, QJsonObject &json)
{
    QJsonObject obj;
    obj.insert("ip", packet->ip->get());
    obj.insert("dev_num", packet->devNum); //

    QString devType = PduDTName::bulid()->getName(packet->devNum);
    obj.insert("dev_type", devType);
    obj.insert("dev_spec", packet->devSpec);

    QString name = packet->info->type->name->get();
    obj.insert("dev_name", name);

    json.insert("Head", QJsonValue(obj));

    return true;
}


void BuildJson::pduDataUnit(PduDataUnit *unit, int id, double rate, QJsonObject &obj)
{
    int value = unit->value->get(id) ;
    if(value >= 0) obj.insert("value", value / rate);

    value = unit->min->get(id);
    if(value >= 0) obj.insert("min", value / rate);

    value = unit->max->get(id);
    if(value >= 0) obj.insert("max", value / rate);

    value = unit->alarm->get(id);
    if(value >= 0) obj.insert("alarm", value / rate);

    value = unit->crMin->get(id);
    if(value >= 0) obj.insert("critical_min", value / rate);

    value = unit->crMax->get(id);
    if(value >= 0) obj.insert("critical_max", value / rate);

    value = unit->crAlarm->get(id);
    if(value >= 0) obj.insert("critical_alarm", value / rate);
}

void BuildJson::pduObjData(PduObjData *ObjData, int id, QJsonObject &obj)
{
    pduDataUnit(ObjData->vol, id, COM_RATE_VOL, obj);
    pduDataUnit(ObjData->cur, id, COM_RATE_CUR, obj);

    int value = ObjData->pow->get(id);
    if(value >= 0) obj.insert("pow", value / COM_RATE_POW);

    value = ObjData->ele->get(id);
    if(value >= 0) obj.insert("ele", value / COM_RATE_ELE);

    value = ObjData->pf->get(id);
    if(value >= 0) obj.insert("PF", value / COM_RATE_PF);

    value = ObjData->sw->get(id);
    if(value >= 0) obj.insert("switch", value);
}

void BuildJson::pduDevData(PduDevData *devData, QJsonObject &obj)
{

}
