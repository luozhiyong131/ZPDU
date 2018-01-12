#ifndef DEVSETTING_H
#define DEVSETTING_H
#include "netdataanalyze.h"

class DevSetting
{
    DevSetting();
public:
    static DevSetting *bulid();

    void sentData(net_dev_data &pkt);
    void sentData(net_dev_data &pkt, const QString & devTypeName);
    void sentData(net_dev_data &pkt, int devType);

    void sentData(const QString &ip, net_dev_data &pkt);
    void sentData(const QString &ip, net_dev_data &pkt, const QString & devTypeName);
    void sentData(const QString &ip, net_dev_data &pkt, int devType);

private:
    uchar *m_buf;
};

#endif // DEVSETTING_H
