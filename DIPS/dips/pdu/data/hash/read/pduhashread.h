#ifndef PDUHASHREAD_H
#define PDUHASHREAD_H
#include "pduhashdata.h"

PduDevHash *pdu_hd_get(int devType, QString &ip);
PduDataPacket *pdu_hd_get(int devType, QString &ip, int num);
int pdu_hd_getSlaveNum(QString &ip);
PduDevHash *pdu_hd_get(QString &ip);
int pdu_hd_getDevType(QString &ip);
PduDataPacket *pdu_hd_getPacket(QString &ip, int num);
#endif // PDUHASHREAD_H
