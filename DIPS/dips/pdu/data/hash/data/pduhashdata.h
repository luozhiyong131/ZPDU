#ifndef PDUHASHDATA_H
#define PDUHASHDATA_H
#include "pduhaship.h"


int pdu_hashData_size(void);
bool pdu_hashData_del(int type);
PduHashIP *pdu_hashData_get(int type);
PduHashIP *pdu_hashData_getHash(int type);
int pdu_hashData_list(QList<int> &list);

#endif // PDUHASHDATA_H
