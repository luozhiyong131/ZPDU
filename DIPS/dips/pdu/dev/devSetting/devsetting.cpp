#include "devsetting.h"
#include "netpackdata.h"
#include "dev/devType/pdudtname.h"
#include "common.h"

#ifdef ZEBRA_MODULE
#include "zebra/zebra_client.h"
#endif

DevSetting::DevSetting()
{
    m_buf = new uchar[256];
}

DevSetting *DevSetting::bulid()
{
    static DevSetting* sington = nullptr;
    if(sington == nullptr)
        sington = new DevSetting();
    return sington;
}


void DevSetting::sentData(net_dev_data &pkt, int devType)
{
    int len = net_data_packets(devType, TRA_TYPR_UDP, &pkt, m_buf);
    udp_queue_append(m_buf, len);
}

void DevSetting::sentData(net_dev_data &pkt, const QString & devTypeName)
{
    int devType = PduDTName::bulid()->getNum(devTypeName);
    sentData(pkt, devType);
}

void DevSetting::sentData(net_dev_data &pkt)
{
    sentData(pkt, PDU_TYPE_ZPDU);
}

void DevSetting::sentData(const QString &ip, net_dev_data &pkt, int devType)
{
    int len = net_data_packets(devType, TRA_TYPR_UDP, &pkt, m_buf);

#ifdef ZEBRA_MODULE
    if(ConfigBase::bulid()->getDownMode() == PDU_Down_TCP){ //TCP
        udp_sent_data(ip, m_buf, len);
    }else if(ConfigBase::bulid()->getDownMode() == PDU_Down_Zebra){ //Zebar
        zebra_client::get_instance()->send_new_protocol_data(ip, m_buf, len);
    }else if(ConfigBase::bulid()->getDownMode() == PDU_Down_TcpZebra){ //双因子
        udp_sent_data(ip, m_buf, len);
        zebra_client::get_instance()->send_new_protocol_data(ip, m_buf, len);
    }
#else
     udp_sent_data(ip, m_buf, len);
#endif

}

void DevSetting::sentData(const QString &ip, net_dev_data &pkt, const QString & devTypeName)
{
    int devType = PduDTName::bulid()->getNum(devTypeName);
    sentData(ip, pkt, devType);
}

void DevSetting::sentData(const QString &ip, net_dev_data &pkt)
{
    sentData(ip, pkt, PDU_TYPE_ZPDU);
}
