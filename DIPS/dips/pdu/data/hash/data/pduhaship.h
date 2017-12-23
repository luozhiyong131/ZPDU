#ifndef PDUHASHIP_H
#define PDUHASHIP_H
#include "pdudevhash.h"

class PduHashIP
{
public:
    PduHashIP();
    ~PduHashIP();

    int size(void);
    bool del(const QString &ip);
    PduDevHash *get(const QString &ip);
    PduDevHash *getDev(const QString &ip);
    int list(QStringList &list);

protected:
    void add(const QString &ip);

private:
    QReadWriteLock  *mLock;
    QHash<QString,PduDevHash *> mHash;
    PduDevHash *mDev;
};

#endif // PDUHASHIP_H
