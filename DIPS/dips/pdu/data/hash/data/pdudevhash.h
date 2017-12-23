#ifndef PDUDEVHASH_H
#define PDUDEVHASH_H
#include "pdudatapacket.h"

class PduDevHash
{
public:
    PduDevHash();
    ~PduDevHash();

    int size(void);
    bool del(int num);
    PduDataPacket *getPacket(int num);
    PduDataPacket *get(int num);
    int list(QList<int> &list);

    PduDataPacket *getMaster(void);
    bool isNew; // 是否是新设备

protected:
    void add(int num);

private:
    QReadWriteLock  *mLock;
    QHash<int,PduDataPacket *> mHash; //以设备号保存设备数据
    PduDataPacket *mPacket;
};

#endif // PDUDEVHASH_H
