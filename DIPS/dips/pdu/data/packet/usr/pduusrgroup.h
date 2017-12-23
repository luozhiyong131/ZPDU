#ifndef PDUUSRGROUP_H
#define PDUUSRGROUP_H
#include <QtCore>

class PduUsrGroup
{
public:
    PduUsrGroup();
    ~PduUsrGroup();

    void del(void);
    void del(const QString &group);
    void set(const QString &group, const int data);

    int size(void);
    int getGroup(QStringList &group);
    bool get(const QString &group, int &data);

private:
    QReadWriteLock  *mLock;
    QHash<QString, int> mHash;
};

#endif // PDUUSRGROUP_H
