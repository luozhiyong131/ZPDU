#ifndef PDUSTRBASE_H
#define PDUSTRBASE_H
#include <QtCore>

class PduStrBase
{
public:
    PduStrBase();
    ~PduStrBase();

    void set(const QString &str);
    void get(QString &str);
    QString get(void);
    void clear(void);

private:
    QString mStr;
    QReadWriteLock  *mLock;
};

#endif // PDUSTRBASE_H
