#include "pduhashread.h"



PduDevHash *pdu_hd_get(int devType, QString &ip)
{
    PduHashIP *hashIP =  pdu_hashData_get(devType); // 根据设备代号获取对应的Hash节点
    return hashIP->get(ip); // 根据不同的设备IP 获取对应的Hash节点
}


/**
 * @brief 根据设备种类、设备IP、设备号来查找设备数据
 * @param devType
 * @param ip
 * @param num
 * @return
 */
PduDataPacket *pdu_hd_get(int devType, QString &ip, int num)
{
    PduDevHash *devHash = pdu_hd_get(devType, ip);

    return devHash->get(num); // 根据不同的设备号 获取对应的设备Hash节点
}


/**
 * @brief 根据IP 获取设备数据
 * @param ip
 * @return
 */
PduDevHash *pdu_hd_get(QString &ip)
{
    QList<int> list;
    pdu_hashData_list(list);

    PduDevHash *devHash = NULL;
    for(int i=0; i<list.size(); ++i)
    {
        int devType = list.at(i);
        PduHashIP *hashIP =  pdu_hashData_getHash(devType);
        if(hashIP)
        {
            PduDevHash *hash = hashIP->getDev(ip);
            if(hash) {
                devHash = hash;
                /**
                 * 优先找在线设备，如果设备不在线则继续查找
                 * 主要解决，当不同类型的设备，存在同一IP时
                 */
                if(hash->getMaster()) {
                    if(hash->getMaster()->offLine > 0)
                        break;
                }
            }
        }
    }

    return devHash;
}


/**
 * @brief 获取设备副机
 * @param ip
 * @return
 */
int pdu_hd_getSlaveNum(QString &ip)
{
    PduDevHash *devHash = pdu_hd_get(ip);

    return devHash->size();
}

/**
 * @brief 获取设备类型
 * @param ip
 * @return
 */
int pdu_hd_getDevType(QString &ip)
{
     int devType = -1;

     PduDevHash *devHash = pdu_hd_get(ip);
     if(devHash) {
         devType = devHash->getMaster()->devType;
     }

    return devType;
}

/**
 * @brief 获取数据包
 * @param ip
 * @param num
 * @return
 */
PduDataPacket *pdu_hd_getPacket(QString &ip, int num)
{
    PduDevHash *devHash = pdu_hd_get(ip);
    if(devHash)
        return devHash->getPacket(num);

    return NULL;
}





