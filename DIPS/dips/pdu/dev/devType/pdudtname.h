#ifndef PDUDTNAME_H
#define PDUDTNAME_H
#include "pduhashdata.h"

#define PDU_TYPE_IP_PDU     0x01010101
#define PDU_TYPE_M_PDU     0x01020101
#define PDU_TYPE_NPM_PDU    0x01030101
#define PDU_TYPE_RPDU       0x01040101
#define PDU_TYPE_ZPDU       0x01050101

class PduDTName
{
public:
    PduDTName();

    int list(QList<int> &list);
    int defineList(QList<int> &list);

    bool getName(int type, QString &name);
    int getNum(const QString &name);
};

#endif // PDUDTNAME_H
