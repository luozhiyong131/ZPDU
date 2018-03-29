#include <QDebug>
#include <QMutexLocker>
#include <QMutex>

#include <atomic>

#include "signaldispatch.h"
#include "zebra_delegates.h"
#include "utils.h"
#include "common_debug.h"
#include "pdu/data/hash/read/pduhashread.h"
#include "zebrdatasql.h"

//#include "pdudevlistsave.h"

extern peersafe::im::Message_client &im;
extern std::set<std::string> g_dedup_set;
extern std::map<std::string, friend_info> friend_map;
extern QMutex friend_map_lock;

zebra_data_quene *zebra_data_quene::m_instance = nullptr;
zebra_process_thread_stroage *zebra_process_thread_stroage::m_intance = nullptr;

zebra_delegates::zebra_delegates()
{

}

zebra_delegates::~zebra_delegates()
{

}

//被对方加好友的回调
/**
 * @brief zebra_delegates::friend_request_via_qr
 * @param qr_channel_id
 * @param info
 * @param friend_channel_id
 * @note invoked when being added as a friend
 */
void zebra_delegates::friend_request_via_qr(
        const string &qr_channel_id,
        const string &info,
        const string &friend_channel_id)
{
    DbZebraChannelItem item;
    item.channel_id = QByteArray::fromStdString(friend_channel_id);
    ZebraChannelDb::get_instance()->insert_to_zebra_channel(item);

    QMutexLocker locker(&friend_map_lock);
    friend_map.insert(std::make_pair(friend_channel_id, friend_info(string(""), 0)));

    g_io_service.Send([friend_channel_id]()
                        {
                                int ret = im.handle_friend_request(friend_channel_id, true);

                                MY_DEBUG(1, GREEN) << "passive add friend result = " << ret << DEBUG_END;

                        });
}

//加对方好友，对方的反馈后的回调
/**
 * @brief zebra_delegates::friend_request_reply
 * @param friend_channel_id
 * @param accepted
 * @note invoked when your add_friend_request has been replied
 */
void zebra_delegates::friend_request_reply(
        const string &friend_channel_id,
        bool accepted)
{
    if(!accepted)
    {

        DbZebraChannelItem item;
        item.channel_id = QByteArray::fromStdString(friend_channel_id);
        QVector<DbZebraChannelItem> qvec = ZebraChannelDb::get_instance()->select_zebra_channel_by_chennel_id(item.channel_id);

        QMutexLocker locker(&friend_map_lock);
        foreach (auto it, qvec)
        {
            ZebraChannelDb::get_instance()->remove_from_zebra_channel(it);
            friend_map.erase(it.channel_id.toStdString());
        }
    }
    else
    {
        DbZebraChannelItem item;
        item.channel_id = QByteArray::fromStdString(friend_channel_id);
        QVector<DbZebraChannelItem> qvec = ZebraChannelDb::get_instance()->select_zebra_channel_by_chennel_id(item.channel_id);

        QMutexLocker locker(&friend_map_lock);
        foreach (auto it, qvec)
        {
            it.channel_state = 0;
            ZebraChannelDb::get_instance()->update_to_zebra_channel(it);
            auto iter = friend_map.find(it.channel_id.toStdString());
            if(iter != friend_map.end())
            {
                iter->second.channel_state = 0;
            }

            MY_DEBUG(1, GREEN) << "active add friend " << it.channel_name.toStdString()
                               << " result = " << accepted << DEBUG_END;
        }
    }
}

/**
 * @brief zebra_delegates::friend_deleted
 * @param friend_channel_id
 * @note invoked when a friend was deleted
 */
void zebra_delegates::friend_deleted(const string &friend_channel_id)
{
    DbZebraChannelItem item;
    item.channel_id = QByteArray::fromStdString(friend_channel_id);
    QVector<DbZebraChannelItem> qvec = ZebraChannelDb::get_instance()->select_zebra_channel_by_chennel_id(item.channel_id);

    QMutexLocker locker(&friend_map_lock);
    foreach (auto it, qvec)
    {
        ZebraChannelDb::get_instance()->remove_from_zebra_channel(it);
        im.delete_friend(friend_channel_id, false);
        friend_map.erase(it.channel_id.toStdString());
    }

    return;
}

/**
 * @brief zebra_delegates::store_data
 * @param key_id - friend channel rsa key's hash
 * @param store_data - friend channel rsa key
 * @note store friend channel's key
 */
void zebra_delegates::store_data(
        const string &key_id,
        const string &store_data)
{
    if(key_id == ICE_CACHE_KEY)
    {
        return;
    }

    ZebraKeyItem keyItem;
    keyItem.key = QByteArray::fromStdString(key_id);
    keyItem.data = QByteArray::fromStdString(store_data);
    ZebraKeyDataDb::get_instance()->insert_to_zebra_key_data(keyItem);

    return;
}

/**
 * @brief zebra_delegates::read_data
 * @param key_id
 * @param read_data
 * @note read friend channel's key
 */
void zebra_delegates::read_data(const string &key_id, string &read_data)
{
    QVector<ZebraKeyItem> qvec_key;

    qvec_key = ZebraKeyDataDb::get_instance()->select_zebra_key_data_by_key(QByteArray::fromStdString(key_id));
    if(qvec_key.empty())
    {
        MY_DEBUG(2, YELLOW) << "read key data fail" << DEBUG_END;
    }
    else
    {
        read_data = qvec_key[0].data.toStdString();
    }

    return ;
}

/**
 * @brief zebra_delegates::delete_data
 * @param key_id
 */
void zebra_delegates::delete_data(const string &key_id)
{
    QVector<ZebraKeyItem> qvec_key = ZebraKeyDataDb::get_instance()->select_zebra_key_data_by_key(QByteArray::fromStdString(key_id));
    if(!qvec_key.empty())
    {
        foreach(auto &item, qvec_key)
        {
            ZebraKeyDataDb::get_instance()->delete_from_zebra_key_data(item);
        }
    }

    return ;
}

/**
 * @brief zebra_delegates::handler_write_message_reply
 * @param channel_id
 * @param message_id
 * @param ret_code
 * @note invoked when ur msg was sent suc/fail/expired.
 */
void zebra_delegates::handler_write_message_reply(
        const std::string &channel_id,
        unsigned long message_id,
        int ret_code)
{
    MY_DEBUG(1, GREEN) << "message_id = " << message_id
                       << " ret_code = " << ret_code
                       << DEBUG_END;

    return ;
}

/**
 * @brief zebra_delegates::isExisted
 * @param item
 * @param expire
 * @return
 * @note filter repeated msg
 */
bool zebra_delegates::isExisted(const string &item, unsigned int expire)
{
    return !g_dedup_set.insert(item).second;
}

/**
 * @brief zebra_delegates::friend_one_oid_message
 * @param friend_channel_id
 * @param message_id
 * @param oid_info
 * @note invoked when received a friend_one_oid_message
 */
void zebra_delegates::friend_one_oid_message(
    const string &friend_channel_id,
    const unsigned long message_id,
    const peersafe::im::OidInfo& oid_info)
{
    /*MY_DEBUG(1, GREEN) << "Result - message_id :" << message_id
        << ", oid  :" << oid_info.oid
        << ", type :" << oid_info.type
        << ", data_type:" << oid_info.data_type << DEBUG_END;*/

    struct zebra_oid_data data_tmp;
    data_tmp.channel_id = friend_channel_id;
    data_tmp.info.push_back(oid_info);
    zebra_data_quene::get_instance()->add_data_to_quene(data_tmp);

    return ;
}

/**
 * @brief zebra_delegates::friend_multi_oid_message
 * @param friend_channel_id
 * @param message_id
 * @param vecOidInfo
 * @note invoked when received a friend_multi_oid_message
 */
void zebra_delegates::friend_multi_oid_message(
    const string &friend_channel_id,
    const unsigned long message_id,
    const vector<peersafe::im::OidInfo> &vecOidInfo)
{
    struct zebra_oid_data data_tmp;
    data_tmp.channel_id = friend_channel_id;
    data_tmp.info.assign(vecOidInfo.begin(), vecOidInfo.end());
    zebra_data_quene::get_instance()->add_data_to_quene(data_tmp);

    return ;
}



zebra_data_quene::~zebra_data_quene()
{

}

zebra_data_quene::zebra_data_quene()
{
    data_stroage.clear();
}

zebra_data_quene *zebra_data_quene::get_instance()
{
    if(nullptr == m_instance)
    {
        m_instance = new zebra_data_quene;
    }

    return m_instance;
}

int zebra_data_quene::get_data_from_quene(struct zebra_oid_data &data)
{
    m_lock.lockForRead();

    if(0 == data_stroage.size())
    {
        m_lock.unlock();
        return -1;
    }

    m_lock.unlock();

    m_lock.lockForWrite();

    if(0 == data_stroage.size())
    {
        m_lock.unlock();
        return -1;
    }

    data.channel_id = (data_stroage.front()).channel_id;
    data.info.assign(((data_stroage.front()).info).begin(), ((data_stroage.front()).info).end());
    data_stroage.pop_front();

    m_lock.unlock();

    return 0;
}

int zebra_data_quene::add_data_to_quene(struct zebra_oid_data &data)
{
    m_lock.lockForWrite();

    data_stroage.push_back(data);

    m_lock.unlock();

    return 0;

}



zebra_data_process_thread::zebra_data_process_thread(QObject *parent)
{

}

zebra_data_process_thread::~zebra_data_process_thread()
{
    quit();
    wait();
}

void zebra_data_process_thread::run()
{
    while(1)
    {
        struct zebra_oid_data data_tmp;
        int ret = zebra_data_quene::get_instance()->get_data_from_quene(data_tmp);
        if(0 == ret)
        {
            QString channel_name;
            {
                QMutexLocker locker(&friend_map_lock);
                auto it = friend_map.find(data_tmp.channel_id);
                if(it != friend_map.end())
                {
                    channel_name = QString::fromStdString(it->second.channel_name);
                }
            }

            if(channel_name.isEmpty())
            {
                MY_DEBUG(2, YELLOW) << "cannot find friend name!" << DEBUG_END;
                continue ;
            }

            for (int i = 0; i < data_tmp.info.size(); i++)
            {
                /*MY_DEBUG(1, GREEN) << "[multi] i:" << i
                     << "  oid:" << data_tmp.info[i].oid
                     << "  type:" << data_tmp.info[i].type
                     << "  data_type:" << data_tmp.info[i].data_type
                     << DEBUG_END;*/

                int slave_num = data_tmp.info[i].type;
                std::string data_str = ((data_tmp.info[i]).value);
                analyze_pdu_trap_data trap_data_tmp(channel_name, slave_num, data_str);
                int ret = trap_data_tmp.get_all_data(channel_name, slave_num);
                if(0 != ret)
                {
                    MY_DEBUG(2, YELLOW) << "trap data analyze error! ip = "
                                        << channel_name.toStdString()
                                        << DEBUG_END;
                }
            }
            msleep(5);
        }
        else
        {
            msleep(10);
        }
    }
}



zebra_process_thread_stroage::zebra_process_thread_stroage(int thread_num)
{
    int i = 0;
    for(i = 0; i < thread_num; i++)
    {
        zebra_data_process_thread *tmp = new zebra_data_process_thread();
        tmp->start();
        m_process.push_back(tmp);

    }
}

zebra_process_thread_stroage::~zebra_process_thread_stroage()
{

}

zebra_process_thread_stroage *zebra_process_thread_stroage::get_intance()
{
    if(nullptr == m_intance)
    {
        m_intance = new zebra_process_thread_stroage(ZEBRA_DATA_THREAD_NUM);
    }

    return m_intance;
}



analyze_pdu_trap_data::analyze_pdu_trap_data(QString channel_name,
                                             int slave_num,
                                             std::string data)
    :m_packet(nullptr)
{
    memset(&m_pdu_data, 0, sizeof(pdudata));
    memcpy(&m_pdu_data, data.c_str(), sizeof(pdudata));
}

int analyze_pdu_trap_data::get_all_data(QString ip, int slave_num)
{
    m_packet = pdu_hd_getPacket(ip.remove("\n"), slave_num);
    if(nullptr == m_packet)
    {
        PduHashIP* hashIP = nullptr;
        PduDevHash* devHash = nullptr;
        PduDataPacket* dev = nullptr;
        int dev_type = 0x01050101;

        hashIP = pdu_hashData_get(dev_type);
        if(NULL == hashIP)
        {
            MY_DEBUG(2, YELLOW) << "get ZPDU hashIP error!" << DEBUG_END;

            return 1;
        }

        devHash = hashIP->get(ip.remove("\n"));
        if(nullptr == devHash)
        {
            MY_DEBUG(2, YELLOW) << "get " << ip.toStdString()
                                << " devHash error!" << DEBUG_END;

            return 2;
        }

        dev = devHash->get(slave_num);
        if(nullptr == dev)
        {
            MY_DEBUG(2, YELLOW) << "get " << ip.toStdString()
                                << " slave_num " << slave_num
                                << "error!" << DEBUG_END;

            return 3;
        }

        dev->devType = dev_type;
        dev->devNum = slave_num;
        dev->ip->set(ip.remove("\n"));
        dev->offLine = PDU_OFF_LINE_TIME;

        if(devHash->isNew)
        {
            dev->info->addr->group->set("ZPDU");
        }

        m_packet = pdu_hd_getPacket(ip.remove("\n"), slave_num);
        if(nullptr == m_packet)
        {
            MY_DEBUG(2, YELLOW) << "get " << ip.toStdString()
                                << " m_packet error!" << DEBUG_END;

            return 4;
        }
    }

    get_is_master_flag();
    get_pdu_set_cmd_channel();
    get_pdu_trap_channel();
    get_pdu_device_type();
    get_output_max_bum();
    get_device_name();
    get_device_addr();
    get_device_ip();
    get_device_mac();

    get_pdu_max_temperature();
    get_pdu_min_temperature();
    get_pdu_cur_temperature();
    get_pdu_up_critical_temperature();
    get_pdu_down_critical_temperature();

    get_pdu_max_hunidity();
    get_pdu_min_hunidity();
    get_pdu_cur_hunidity();
    get_pdu_up_critical_hunidity();
    get_pdu_down_critical_hunidity();

    get_pdu_door_state();
    get_pdu_smoke_state();
    get_dpu_water_state();

    get_total_current();
    get_total_max_current();
    get_total_min_current();
    get_total_up_critical_current();
    get_total_down_critical_current();

    get_total_voltage();
    get_total_max_voltage();
    get_total_min_voltage();
    get_total_up_critical_voltage();
    get_total_down_critical_voltage();

    get_total_electric_energy();
    get_pdu_power_factor();

    get_pdu_output_cur_current();
    get_pdu_output_min_current();
    get_pdu_output_max_current();
    get_pdu_output_max_critical_current();
    get_pdu_output_min_critical_current();
    get_pdu_output_power_factor();
    get_pdu_output_electric_energy();
    get_pdu_output_switch_state();
    get_pdu_output_name();

    m_packet->offLine = PDU_OFF_LINE_TIME*3;
    //SignalDispatch::get()->dispatch(m_packet);

    return 0;
}

int analyze_pdu_trap_data::get_is_master_flag()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    //MY_DEBUG(1, GREEN) << "ms : " << m_pdu_data.ms << DEBUG_END;

    return 0;
}

int analyze_pdu_trap_data::get_pdu_set_cmd_channel()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    //MY_DEBUG(1, GREEN) << "set channel : " << m_pdu_data.comType << DEBUG_END;

    return 0;
}

int analyze_pdu_trap_data::get_pdu_trap_channel()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    //MY_DEBUG(1, GREEN) << "set channel : " << m_pdu_data.trap_ch << DEBUG_END;

    return 0;
}

int analyze_pdu_trap_data::get_pdu_device_type()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    m_packet->devType = 0x01050101;

    return 0;
}

int analyze_pdu_trap_data::get_output_max_bum()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    //MY_DEBUG(1, GREEN) << "output num : " << m_pdu_data.outputnum << DEBUG_END;

    return 0;
}

QString analyze_pdu_trap_data::get_device_name()
{
    QString str_tmp;
    if(nullptr == m_packet)
    {
        return "";
    }

    m_packet->info->type->name->clear();
    m_packet->info->type->name->set(QString(QLatin1String(m_pdu_data.devicename)));

    std::string pdu_name;
    pdu_name.assign(m_pdu_data.devicename, 32);
    //MY_DEBUG(1, GREEN) << "device name : " << pdu_name << DEBUG_END;

    return str_tmp;
}

QString analyze_pdu_trap_data::get_device_addr()
{
    QString str_tmp;
    if(nullptr == m_packet)
    {
        return "";
    }

    std::string pdu_addr;
    pdu_addr.assign(m_pdu_data.deviceaddr, 32);
    //MY_DEBUG(1, GREEN) << "device address : " << pdu_addr << DEBUG_END;

    return str_tmp;
}

QString analyze_pdu_trap_data::get_device_ip()
{
    QString str_tmp;
    if(nullptr == m_packet)
    {
        return "";
    }

    m_packet->ip->set(QString(QLatin1String(m_pdu_data.ipaddr)).remove("\n"));

    std::string pdu_ip;
    pdu_ip.assign(m_pdu_data.ipaddr, 40);
    //MY_DEBUG(1, GREEN) << "device ip : " << pdu_ip << DEBUG_END;

    return str_tmp;
}

QString analyze_pdu_trap_data::get_device_mac()
{
    QString str_tmp;
    if(nullptr == m_packet)
    {
        return "";
    }

    std::string pdu_mac;
    pdu_mac.assign(m_pdu_data.ipaddr, 20);
    //MY_DEBUG(1, GREEN) << "device mac : " << pdu_mac << DEBUG_END;

    return str_tmp;
}



int analyze_pdu_trap_data::get_pdu_max_temperature()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_TEMP_SENSOR_MAX_NUM; i++)
    {
        m_packet->data->env->tem->max->set(i, (m_pdu_data.maxT[i])*COM_RATE_TEM);

        /*MY_DEBUG(1, GREEN) << "temperature : " << i
                           << " max : " <<  m_pdu_data.maxT[i]
                           << DEBUG_END;*/
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_min_temperature()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_TEMP_SENSOR_MAX_NUM; i++)
    {
        m_packet->data->env->tem->min->set(i, (m_pdu_data.minT[i])*COM_RATE_TEM);

        /*MY_DEBUG(1, GREEN) << "temperature : " << i
                           << " min : " <<  m_pdu_data.minT[i]
                           << DEBUG_END;*/
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_cur_temperature()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_TEMP_SENSOR_MAX_NUM; i++)
    {
        m_packet->data->env->tem->value->set(i, (m_pdu_data.temp[i])*COM_RATE_TEM);

        /*MY_DEBUG(1, GREEN) << "temperature : " << i
                           << " cur : " <<  m_pdu_data.temp[i]
                           << DEBUG_END;*/
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_up_critical_temperature()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_TEMP_SENSOR_MAX_NUM; i++)
    {
        m_packet->data->env->tem->crMax->set(i, (m_pdu_data.ucritT[i])*COM_RATE_TEM);

        /*MY_DEBUG(1, GREEN) << "temperature : " << i
                           << " up critical : " <<  m_pdu_data.ucritT[i]
                           << DEBUG_END;*/
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_down_critical_temperature()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_TEMP_SENSOR_MAX_NUM; i++)
    {
        m_packet->data->env->tem->crMin->set(i, (m_pdu_data.dcritT[i])*COM_RATE_TEM);

        /*MY_DEBUG(1, GREEN) << "temperature : " << i
                           << " down critical : " <<  m_pdu_data.dcritT[i]
                           << DEBUG_END;*/
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_max_hunidity()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_HUMI_SENSOR_MAX_NUM; i++)
    {
        m_packet->data->env->hum->max->set(i, (m_pdu_data.maxH[i])*COM_RATE_HUM);

        /*MY_DEBUG(1, GREEN) << "hunidity : " << i
                           << " max : " <<  m_pdu_data.maxH[i]
                           << DEBUG_END;*/
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_min_hunidity()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_HUMI_SENSOR_MAX_NUM; i++)
    {
        m_packet->data->env->hum->min->set(i, (m_pdu_data.minH[i])*COM_RATE_HUM);

        /*MY_DEBUG(1, GREEN) << "hunidity : " << i
                           << " min : " <<  m_pdu_data.minH[i]
                           << DEBUG_END;*/
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_cur_hunidity()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_HUMI_SENSOR_MAX_NUM; i++)
    {
        m_packet->data->env->hum->value->set(i, (m_pdu_data.hum[i])*COM_RATE_HUM);

        /*MY_DEBUG(1, GREEN) << "hunidity : " << i
                           << " cur : " <<  m_pdu_data.hum[i]
                           << DEBUG_END;*/
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_up_critical_hunidity()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_HUMI_SENSOR_MAX_NUM; i++)
    {
        m_packet->data->env->hum->crMax->set(i, (m_pdu_data.ucritH[i])*COM_RATE_HUM);

        /*MY_DEBUG(1, GREEN) << "hunidity : " << i
                           << " up critical : " <<  m_pdu_data.ucritH[i]
                           << DEBUG_END;*/
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_down_critical_hunidity()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_HUMI_SENSOR_MAX_NUM; i++)
    {
        m_packet->data->env->hum->crMin->set(i, (m_pdu_data.dcritH[i])*COM_RATE_HUM);

        /*MY_DEBUG(1, GREEN) << "hunidity : " << i
                           << " down critical : " <<  m_pdu_data.dcritH[i]
                           << DEBUG_END;*/
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_door_state()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_DOOR_SENSOR_MAX_NUM; i++)
    {
        m_packet->data->env->door->set(i, m_pdu_data.door[i]);

        /*MY_DEBUG(1, GREEN) << "door : " << i
                           << " state : " <<  m_pdu_data.door[i]
                           << DEBUG_END;*/
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_smoke_state()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    m_packet->data->env->smoke->set(0, m_pdu_data.smoke);

    //MY_DEBUG(1, GREEN) << "smoke state : " <<  m_pdu_data.smoke << DEBUG_END;

    return 0;
}

int analyze_pdu_trap_data::get_dpu_water_state()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    m_packet->data->env->water->set(0, m_pdu_data.water);

    //MY_DEBUG(1, GREEN) << "water state : " <<  m_pdu_data.water << DEBUG_END;

    return 0;
}



int analyze_pdu_trap_data::get_total_current()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    PduObjData *line = m_packet->data->line;
    int i = 0;
    for(i = 0; i < ZPDU_LINE_MAX_NUM; i++)
    {
        line->cur->value->set(i, m_pdu_data.Tcur[i]);
    }

    return 0;
}

int analyze_pdu_trap_data::get_total_max_current()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    PduObjData *line = m_packet->data->line;
    int i = 0;
    for(i = 0; i < ZPDU_LINE_MAX_NUM; i++)
    {
        line->cur->max->set(i, m_pdu_data.maxTcur[i]);
    }

    return 0;
}

int analyze_pdu_trap_data::get_total_min_current()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    PduObjData *line = m_packet->data->line;
    int i = 0;
    for(i = 0; i < ZPDU_LINE_MAX_NUM; i++)
    {
        line->cur->min->set(i, m_pdu_data.minTcur[i]);
    }

    return 0;
}

int analyze_pdu_trap_data::get_total_up_critical_current()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    PduObjData *line = m_packet->data->line;
    int i = 0;
    for(i = 0; i < ZPDU_LINE_MAX_NUM; i++)
    {
        line->cur->crMax->set(i, m_pdu_data.maxTCcrit[i]);
    }

    return 0;
}

int analyze_pdu_trap_data::get_total_down_critical_current()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    PduObjData *line = m_packet->data->line;
    int i = 0;
    for(i = 0; i < ZPDU_LINE_MAX_NUM; i++)
    {
        line->cur->crMin->set(i, m_pdu_data.minTCcrit[i]);
    }

    return 0;
}

int analyze_pdu_trap_data::get_total_voltage()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    PduObjData *line = m_packet->data->line;
    int i = 0;
    for(i = 0; i < ZPDU_LINE_MAX_NUM; i++)
    {
        line->vol->value->set(i, m_pdu_data.Tvol[i]);
    }

    return 0;
}

int analyze_pdu_trap_data::get_total_max_voltage()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    PduObjData *line = m_packet->data->line;
    int i = 0;
    for(i = 0; i < ZPDU_LINE_MAX_NUM; i++)
    {
        line->vol->max->set(i, (m_pdu_data.maxTVol[i])*COM_RATE_VOL);
    }

    return 0;
}

int analyze_pdu_trap_data::get_total_min_voltage()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    PduObjData *line = m_packet->data->line;
    int i = 0;
    for(i = 0; i < ZPDU_LINE_MAX_NUM; i++)
    {
        line->vol->min->set(i, (m_pdu_data.minTVol[i])*COM_RATE_VOL);
    }

    return 0;
}

int analyze_pdu_trap_data::get_total_up_critical_voltage()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    PduObjData *line = m_packet->data->line;
    int i = 0;
    for(i = 0; i < ZPDU_LINE_MAX_NUM; i++)
    {
        line->vol->crMax->set(i, (m_pdu_data.maxTVcrit[i])*COM_RATE_VOL);
    }

    return 0;
}

int analyze_pdu_trap_data::get_total_down_critical_voltage()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    PduObjData *line = m_packet->data->line;
    int i = 0;
    for(i = 0; i < ZPDU_LINE_MAX_NUM; i++)
    {
        line->vol->crMin->set(i, (m_pdu_data.minTVcrit[i])*COM_RATE_VOL);
    }

    return 0;
}

int analyze_pdu_trap_data::get_total_electric_energy()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    PduObjData *line = m_packet->data->line;
    int i = 0;
    for(i = 0; i < ZPDU_LINE_MAX_NUM; i++)
    {
        line->ele->set(i, (m_pdu_data.Tkwh[i])*COM_RATE_ELE);

        /*MY_DEBUG(1, YELLOW) << "electric energy " << i
                            << " value : " << m_pdu_data.Tkwh[i]
                            << DEBUG_END;*/
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_power_factor()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    PduObjData *line = m_packet->data->line;
    int i = 0;
    for(i = 0; i < ZPDU_LINE_MAX_NUM; i++)
    {
        line->pf->set(i, (m_pdu_data.Tpowerfactor[i])*COM_RATE_PF);
    }

    return 0;
}




int analyze_pdu_trap_data::get_pdu_output_cur_current()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_OUTPUT_MAX_NUM; i++)
    {
        m_packet->data->output->cur->value->set(i, m_pdu_data.cur[i]);
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_output_min_current()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_OUTPUT_MAX_NUM; i++)
    {
        m_packet->data->output->cur->min->set(i, m_pdu_data.mincur[i]);
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_output_max_current()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_OUTPUT_MAX_NUM; i++)
    {
        m_packet->data->output->cur->max->set(i, m_pdu_data.maxcur[i]);
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_output_max_critical_current()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_OUTPUT_MAX_NUM; i++)
    {
        m_packet->data->output->cur->crMax->set(i, m_pdu_data.maxcrit[i]);
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_output_min_critical_current()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_OUTPUT_MAX_NUM; i++)
    {
        m_packet->data->output->cur->crMin->set(i, m_pdu_data.mincrit[i]);
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_output_power_factor()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_OUTPUT_MAX_NUM; i++)
    {
        m_packet->data->output->pf->set(i, m_pdu_data.powerfactor[i]);
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_output_electric_energy()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_OUTPUT_MAX_NUM; i++)
    {
        m_packet->data->output->ele->set(i, m_pdu_data.kwh[i]);
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_output_switch_state()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_OUTPUT_MAX_NUM; i++)
    {
        m_packet->data->output->sw->set(i, (m_pdu_data.swicth[i])+1);

        /*MY_DEBUG(1, RED) << "switch state : index = " << i+1
                         << " state = " << m_pdu_data.swicth[i]
                         << DEBUG_END;*/
    }

    return 0;
}

int analyze_pdu_trap_data::get_pdu_output_name()
{
    if(nullptr == m_packet)
    {
        return -1;
    }

    int i = 0;
    for(i = 0; i < ZPDU_OUTPUT_MAX_NUM; i++)
    {
        m_packet->output->name->set(i, QString(QLatin1String(m_pdu_data.outputname[i])));

        std::string output_name;
        output_name.assign(m_pdu_data.outputname[i], 32);
        //MY_DEBUG(1, RED) << "output name : " << output_name << DEBUG_END;
    }

    return 0;
}


