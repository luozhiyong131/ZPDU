#ifndef PDUTIMESWITCH_H
#define PDUTIMESWITCH_H
#include "pdustrbase.h"


class PduTimeSwitch
{
public:
    PduTimeSwitch();
    ~PduTimeSwitch();

    void del(void); // 清理所有定时数据， 当分组发生改变时，一定要调用此函数清理数据，保证定时信息同步

    void set(int output, const QString &time);
    QString get(int output); //获取输出位名称
    int size(void); // 输出位的位数

    int sizeGroup(void); // 输出位的位数
    void setGroup(const QString &group, const QString &time);
    QString getGroup(const QString &group); //获取输出位名称

private:
    QReadWriteLock  *mLock;
    QMap<int, QString> mMap; //根据输出位，保存上下电时间  key 输出位， value 输出位名称

    QMap<QString, QString> mGroupMap; // 根据组名，保存定时上下电时间
};

#endif // PDUTIMESWITCH_H
