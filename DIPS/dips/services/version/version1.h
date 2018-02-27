#ifndef VERSION1_H
#define VERSION1_H

#include <QObject>
#include "services/version/versioncommon.h"

class Version1 : public QObject
{
    Q_OBJECT
public:
    explicit Version1(QObject *parent = nullptr);

    /* V1.0 */
    void versionV1_0(QJsonObject  &json); //1.0版本

private:
    void setHeadInfoV1_0(QJsonObject  &json, HeadInfo &headInfo); //设置部分头部信息
    void setlineV1_0(QJsonArray  &jsonArray, HeadInfo &headInfo); //设置输入相
    void setOutPutV1_0(QJsonArray  &jsonArray, HeadInfo &headInfo); //设置输出位
    void setHumV1_0(QJsonArray  &jsonArray, HeadInfo &headInfo); //设置湿度
    void setTemV1_0(QJsonArray  &jsonArray, HeadInfo &headInfo); //设置温度
    void setIpAddrV1_0(QJsonObject  &json, HeadInfo &headInfo); //设置addr

    //common
    bool jsonIsEmpty(QJsonObject  &json, QString name); //判断四个值是否空
    void dataCompletion(QJsonObject  &json, QString name, uchar *data);

    //send
    void sentData(net_dev_data &,  HeadInfo &headInfo);

signals:

public slots:

private:
    VersionCommon *vCommon;
};

#endif // VERSION1_H
