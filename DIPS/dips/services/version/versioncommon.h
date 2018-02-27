#ifndef VERSIONCOMMON_H
#define VERSIONCOMMON_H

#include <QObject>
#include "net/analyze/netdataanalyze.h"

class HeadInfo;

class VersionCommon : public QObject
{
    Q_OBJECT
public:
    explicit VersionCommon(QObject *parent = nullptr);

    /* 发送数据 */
    void sentData(net_dev_data &,  HeadInfo &headInfo);

signals:

public slots:
};

class HeadInfo
{
public:
    QString ip;   //ip
    int devNum; //设备号 0-9
    QString devType;   //设备类型 ZPDU
    int devSpec;   //设备系统 ABCD
   // QString devName;   //设备名称 PDU
};

#endif // VERSIONCOMMON_H
