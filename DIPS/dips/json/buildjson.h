#ifndef BUILDJSON_H
#define BUILDJSON_H
#include "pdu/data/hash/read/pduhashread.h"

class BuildJson
{
public:
    BuildJson();

    bool getJson(int devType, QString &ip, int num);

protected:
    bool head(PduDataPacket *packet, QJsonObject &json);

    void pduDataUnit(PduDataUnit *unit, int id, double rate, QJsonObject &obj);
    void pduObjData(PduObjData *ObjData, int id, QJsonObject &obj);
    void pduDevData(PduDevData *devData, QJsonObject &obj);

    void lineItem();

private:
};

#endif // BUILDJSON_H
