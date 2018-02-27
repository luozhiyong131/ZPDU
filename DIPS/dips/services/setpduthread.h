#ifndef SETPDUTHREAD_H
#define SETPDUTHREAD_H

#include <QThread>
#include <QStringList>
#include "net/analyze/netdataanalyze.h"

#include "services/version/versioncommon.h"
#include "services/version/version1.h"

class HeadInfo;

class SetPduThread : public QThread
{
    Q_OBJECT
public:
    explicit SetPduThread(QObject *parent = nullptr);
    ~SetPduThread();
    static SetPduThread *bulid();

    void addList(const QString &message);

protected:
    void run();

    /* 控制分解 */
    void versionControl(QJsonObject  &json);  //版本控制

//    /* V1.0 */
//    void versionV1_0(QJsonObject  &json); //1.0版本
//    void setlineV1_0(QJsonArray  &jsonArray, HeadInfo &headInfo); //设置输入相
//    void setOutPutV1_0(QJsonArray  &jsonArray, HeadInfo &headInfo); //设置输出位
//    void setHumV1_0(QJsonArray  &jsonArray, HeadInfo &headInfo); //设置湿度
//    void setTemV1_0(QJsonArray  &jsonArray, HeadInfo &headInfo); //设置温度
//    void setIpAddrV1_0(QJsonObject  &json, HeadInfo &headInfo); //设置addr

//    //common
//    bool jsonIsEmpty(QJsonObject  &json, QString name); //判断四个值是否空
//    void dataCompletion(QJsonObject  &json, QString name, uchar *data);

//    /* 发送数据 */
//    void sentData(net_dev_data &,  HeadInfo &headInfo);



private:
    QStringList m_memoryList;
    bool isRun;
    QReadWriteLock lock; //读写锁
    Version1 *version1;

};



#endif // SETPDUTHREAD_H
