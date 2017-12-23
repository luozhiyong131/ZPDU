#ifndef PDUDLOBJECT_H
#define PDUDLOBJECT_H
#include "pdu/data/hash/read/pduhashread.h"


typedef enum {
    PDU_DL_LIST =0,
    PDU_DL_LINE,
    PDU_DL_OFF,
    PDU_DL_ALARM,
}PDU_DL_TYPE;


class PduDLObject
{
public:
    PduDLObject();
    ~PduDLObject();

    int list(QString &ip, QList<int> &numList);
    int line(QString &ip, QList<int> &numList);
    int offLine(QString &ip, QList<int> &numList);
    int alarm(QString &ip, QList<int> &numList);

protected:
    int get(QString &ip, int id, QList<int> &numList);
    void add(PduDataPacket *data,int id, QList<int> &numList);

private:

};

#endif // PDUDLOBJECT_H
