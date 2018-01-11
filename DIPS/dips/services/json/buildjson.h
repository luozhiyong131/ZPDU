#ifndef BUILDJSON_H
#define BUILDJSON_H
#include "pdu/data/hash/read/pduhashread.h"
#include "pdu/dev/devType/pdudtname.h"
#include "common.h"

class BuildJson
{
    BuildJson();
public:
    static BuildJson *bulid();

    bool getJson(int devType, QString &ip, int num, QJsonObject &json);
    bool getJson(PduDataPacket *packet, QJsonObject &json);

    bool saveJson(const QString &name, QJsonObject &json);

protected:
    void head(PduDataPacket *packet, QJsonObject &json);

    void pduDataUnit(PduDataUnit *unit, int id, const QString &str, double rate, QJsonObject &obj);
    void pduObjData(PduObjData *ObjData, int id, QJsonObject &obj);

    void pduLineData(PduObjData *ObjData, QJsonObject &json, int mode);
    void pduOutputData(PduObjData *ObjData, PduOutput *output, QJsonObject &json);

    void pduThData(PduDataUnit *unit, QJsonObject &json, int mode);
    void pduEnvItemData(PduDataBase *data, QJsonObject &json, int mode);
    void pduEnvData(PduEnvData *env, QJsonObject &json);

    void pduDevData(PduDataPacket *packet, QJsonObject &obj);

    void pduIpData(PduNetIPAddr *net, QJsonObject &json);
    void pduNetData(PduNetInfo *net, QJsonObject &obj);
};

#endif // BUILDJSON_H
