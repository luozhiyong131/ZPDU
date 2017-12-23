#ifndef PDUDATABASE_H
#define PDUDATABASE_H
#include <QtCore>

class PduDataBase
{
public:
    PduDataBase();
    ~PduDataBase();

    int size(void);
    bool set(int local, int data);
    int get(int local);
    void setAll(int data);

protected:
    void add(int data);

private:
    QList<int> *mDataList;
    QReadWriteLock  *mLock;
};

#endif // PDUDATABASE_H
