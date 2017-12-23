#ifndef PDUGROUPSINFO_H
#define PDUGROUPSINFO_H
#include <QtCore>

class PduGroupsInfo
{
public:
    PduGroupsInfo();
    ~PduGroupsInfo();

    void del(void);
    void set(int output,const QString &groupName);
    bool get(int output, QString &groupName); //获取组名

    int groupList(QStringList &group); // 获取组名列表
    int group(const QString &groupName, int array[]); // 根据组名来获取输出位列表

private:
    QReadWriteLock  *mLock;
    QMap<int, QString> mMap; //根据输出位，保存组名  key 输出位， value 组名称
};

#endif // PDUGROUPSINFO_H
