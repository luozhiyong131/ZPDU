#ifndef PDUUSRHASH_H
#define PDUUSRHASH_H
#include "pdudevusr.h"

class PduUsrHash
{
public:
    PduUsrHash();
    ~PduUsrHash();

    void del(void);
    void del(const QString &name);
    void setPwd(const QString &name, const QString &pwd);
    void setEmil(const QString &name, int id, const QString &emil);
    void setPhone(const QString &name, const QString &phone);
    void setGroup(const QString &name, const QString &group);

    int size(void);
    int getUsr(QStringList &usr);
    PduDevUsr *get(const QString &name);

protected:
    void addUsr(const QString &name);
    void setUsr(PduDevUsr *usr, PduDevUsr *from);

private:
    QReadWriteLock  *mLock;
    QHash<QString, PduDevUsr*> mHash;
};

#endif // PDUUSRHASH_H
