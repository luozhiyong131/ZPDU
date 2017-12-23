#ifndef PDUDEVLIST_H
#define PDUDEVLIST_H
#include "pdudlobject.h"


class PduDevList
{
public:
    PduDevList();
    ~PduDevList();

    int getList(int type, QStringList &ipList);
    int getLine(int type, QStringList &ipList);
    int getAlarm(int type, QStringList &ipList);
    int getOff(int type, QStringList &ipList);

protected:
    int get(int type, int id, QStringList &list);
    void add(PduDataPacket *data,int id, QStringList &ipList);

private:

};

#endif // PDUDEVLIST_H
