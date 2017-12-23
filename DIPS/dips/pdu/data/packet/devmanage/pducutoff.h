#ifndef PDUCUTOFF_H
#define PDUCUTOFF_H
#include <QtCore>

class PduCutOff
{
public:
    PduCutOff();
    ~PduCutOff();

    void del(void); // 清理所有定时数据， 当分组发生改变时，一定要调用此函数清理数据，保证定时信息同步

    void set(int output,bool en);
    void setAll(bool en);
    bool get(int output, bool &en); //获取输出位名称
    int size(void); // 输出位的位数

    void setGroup(const QString &group,bool en);
    bool getGroup(const QString &group, bool &en); //获取输出位名称
    int sizeGroup(void); // 输出位的位数

private:
    QReadWriteLock  *mLock;
    QMap<int, bool> mMap; //根据输出位，超限断电
    QMap<QString, bool> mGroupMap;
};

#endif // PDUCUTOFF_H
