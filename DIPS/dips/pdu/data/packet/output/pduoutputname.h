#ifndef PDUOUTPUTNAME_H
#define PDUOUTPUTNAME_H
#include "pdustrbase.h"

class PduOutputName
{
public:
    PduOutputName();
    ~PduOutputName();

    void set(int output,const QString &name);
    bool get(int output, QString &name); //获取输出位名称
    void setAll(const QString &name);

    int size(void); // 输出位的位数
    int getList(QStringList &name); // 获取输出位列表

private:
    QReadWriteLock  *mLock;
    QMap<int, QString> mMap; //根据输出位，保存输出位名称  key 输出位， value 输出位名称
};

#endif // PDUOUTPUTNAME_H
