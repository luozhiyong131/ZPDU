#ifndef PDUOUTPUTSORT_H
#define PDUOUTPUTSORT_H
#include <QtCore>

class PduOutputSort
{
public:
    PduOutputSort();
    ~PduOutputSort();

    void set(int output,const int value);
    bool get(int output, int &value); //获取输出位名称

    int size(void); // 输出位的位数

private:
    QReadWriteLock  *mLock;
    QMap<int, int> mMap; //根据输出位，保存输出位名称  key 输出位， value 输出位排序号
};

#endif // PDUOUTPUTSORT_H
