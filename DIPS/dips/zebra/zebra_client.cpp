#include <QDebug>
#include <QMutex>
#include <QMutexLocker>

#include <atomic>
#include <stdint.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <QHostAddress>
#include <QNetworkInterface>

#include "zebra_client.h"
#include "zebra_delegates.h"
#include "utils.h"
#include "common.h"
#include "common_debug.h"
#include "zebra/zebrdatasql.h"
#include "autoaddfriend/autoaddfriend.h"


extern peersafe::im::Message_client &im;
extern peersafe::im::Message_client *_im;
extern std::set<std::string> g_dedup_set;

extern std::map<std::string, friend_info> friend_map;
extern QMutex friend_map_lock;

extern zebra_delegates* im_dele;

extern std::atomic_int zebra_network_state;
extern QString auto_zebra_security_ip;

zebra_client * zebra_client::m_instance = nullptr;
zebra_cmd_index *zebra_cmd_index::m_cmd_instance = nullptr;

/**
 * @brief zebra_client::get_instance
 * @return
 */
zebra_client* zebra_client::get_instance()
{
    if(m_instance == nullptr)
    {
        m_instance = new zebra_client();
    }
    return m_instance;
}



/**
 * @brief zebra_client::zebra_client
 */
zebra_client::zebra_client()
{
    listen_friends_flag = true;

    is_data_process_run = false;
}

/**
 * @brief zebra_client::~zebra_client
 */
zebra_client::~zebra_client()
{
    m_netstate_timer->stop();
    delete m_netstate_timer;

    if(_im)
    {
        delete _im;
    }
}

/**
 * @brief zebra_client::init
 */
void zebra_client::init()
{
    ZebraChannelDb::get_instance();
    ZebraKeyDataDb::get_instance();

    init_friend_map();
    join_network();

    last_jion_time = QDateTime::currentMSecsSinceEpoch();
    m_netstate_timer = new QTimer(this);
    m_netstate_timer->setSingleShot(false);
    QObject::connect(m_netstate_timer, SIGNAL(timeout()), this, SLOT(check_network_state()));
    m_netstate_timer->setInterval(2000);
    m_netstate_timer->start();

}

/**
 * @brief zebra_client::init_friend_map
 */
void zebra_client::init_friend_map()
{
    QVector<DbZebraChannelItem> qvec = ZebraChannelDb::get_instance()->get_all_zerba_chennel();
    {
        QMutexLocker locker(&friend_map_lock);
        foreach(auto it, qvec)
        {
            friend_info info(it.channel_name.toStdString(), it.channel_state);
            friend_map.insert(std::make_pair(it.channel_id.toStdString(), info));
        }
    }

}

/**
 * @brief zebra_client::check_network_state
 */
void zebra_client::check_network_state()
{
    //check network state...
    int network_state = im.get_network_state();
    int duration = im.inactive_time_duration();//ms
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    int last_join_duration = now - last_jion_time;
    if(network_state < 4)
    {
        MY_DEBUG(1, GREEN) << "network_state = " << network_state << DEBUG_END;
    }

    zebra_network_state = network_state;

    if((network_state <= 0 || duration > 10000) && last_join_duration > 10000)
    {
        MY_DEBUG(2, LIGHT_CYAN) << "rejoin network..." << DEBUG_END;
        join_network();
        listen_friends_flag = true;
        last_jion_time = now;
    }

    if(network_state > 3)
    {
        if(listen_friends_flag)
        {
            listen_all_friends();
            listen_friends_flag = false;
        }

        if(!is_data_process_run)
        {
            is_data_process_run = true;
            zebra_process_thread_stroage::get_intance();
        }
    }
}

/**
 * @brief zebra_client::join_network
 */
void zebra_client::join_network()
{
    QString zebra_security = auto_zebra_security_ip;
    QString str_ip_regexp ="((([1-9][0-9]?)|(1[0-9]?[0-9]?)|(2[0-4]?[0-9])|(25?[0-5]))\\.){1}"
                          "((0|([1-9][0-9]?)|(1[0-9]?[0-9]?)|(2[0-4]?[0-9])|(25?[0-5]))\\.){2}"
                          "(0|([1-9][0-9]?)|(1[0-9]?[0-9]?)|(2[0-4]?[0-9])|(25?[0-5])){1}";
    QRegExp rx(str_ip_regexp);
    if(!rx.exactMatch(zebra_security))
    {
        MY_DEBUG(2, YELLOW) << "zebra security IP " << zebra_security.toStdString()
                            << " address is incorrect!" << DEBUG_END;
        return;
    }
    else
    {
        QString local_point;
        QList<QHostAddress> AddressList = QNetworkInterface::allAddresses();
        foreach(QHostAddress address, AddressList)
        {
            if(address.protocol() == QAbstractSocket::IPv4Protocol &&
                    address != QHostAddress::Null &&
                    address != QHostAddress::LocalHost)
            {
                if (address.toString().contains("127.0."))
                {
                    continue;
                }

                local_point = address.toString();
            }
        }

        local_point = local_point + ":20111";
        QString server_address = zebra_security + ":27053";
        QString config_str = local_point + QString(" ") + server_address;

        MY_DEBUG(1, GREEN) << "join zebra net " << config_str.toStdString() << DEBUG_END;

        im.init(config_str.toStdString(), im_dele, " ",  peersafe::im::DEVICE_TYPE_WINDOWS);
    }

}

/**
 * @brief zebra_client::add_friend
 * @param short_code
 * @param friend_name
 * @return suc 0, fail -1
 */
int zebra_client::add_friend(const QString short_code, const QString friend_name)
{
    if(zebra_network_state < 4)
    {
        MY_DEBUG(2, BROWN)<<"zebra_network_state = " << zebra_network_state << DEBUG_END;

        return -1;
    }

    if(short_code.isEmpty())
    {
        return -1;
    }

    std::string shortCode = short_code.toStdString();
    std::string invite_code = im.add_friend_via_short_code(shortCode, "");
    if (invite_code == "no_short_code")
    {
        MY_DEBUG(3, RED)<<"no_short_code" << DEBUG_END;

        return -1;
    }
    else if (invite_code == "connection_fail")
    {
        MY_DEBUG(3, RED)<<"add_device_via_short_code connection_fail 1" << DEBUG_END;
        return -1;
    }
    else
    {
        std::string channel_id = im.add_friend_via_qr_channel(invite_code, "");
        if (channel_id == "no_qr_channel")
        {
            MY_DEBUG(3, RED)<<"no_qr_channel" << DEBUG_END;
            return -1;
        }
        else if (channel_id == "connection_fail")
        {
            MY_DEBUG(3, RED)<<"add_device_via_qr_channel connection_fail 2" << DEBUG_END;
            return -1;
        }
        else
        {
            DbZebraChannelItem it;
            it.channel_id = QByteArray::fromStdString(channel_id);
            it.channel_name = friend_name;
            it.channel_state = -1; //not a friend now
            ZebraChannelDb::get_instance()->insert_to_zebra_channel(it);

            QMutexLocker locker(&friend_map_lock);
            friend_info info(it.channel_name.toStdString(), it.channel_state);
            friend_map.insert(std::make_pair(it.channel_id.toStdString(), info));
            MY_DEBUG(1, GREEN)<<"start add-friend , waiting......" << DEBUG_END;

        }
    }
    return 0;
}

/**
 * @brief zebra_client::gen_short_code
 * @return QString type short code
 */
QString zebra_client::gen_short_code()
{
    string qr_channel = im.generate_qr_channel(600);
    string short_code = im.generate_short_code(qr_channel, 600);

    return QString::fromStdString(short_code);
}

/**
 * @brief zebra_client::listen_all_friends
 * @return
 */
int zebra_client::listen_all_friends()
{
    QMutexLocker locker(&friend_map_lock);
    MY_DEBUG(1, GREEN)<<"friend_map size = " << friend_map.size() << DEBUG_END;
    for(const auto &it : friend_map)
    {
        im.listen_friend(it.first);
    }

    return 0;
}

/**
 * @brief zebra_client::send_oids_request
 * @param channel_id
 * @param qvec_oids
 * @return suc 0, fail -1
 */
int zebra_client::send_oids_request(const QByteArray channel_id, const QVector<peersafe::im::OidInfo> qvec_oids)
{
    if(!channel_id.isEmpty())
    {
        im.send_multi_oid_message(channel_id.toStdString(), qvec_oids.toStdVector(), 30, 0);
    }
    else
    {
        MY_DEBUG(2, BROWN)<<"channel_id is empty" << DEBUG_END;

        return -1;
    }

    return 0;
}

/**
 * @brief zebra_client::send_one_oid_request
 * @param channel_id
 * @param oid_info
 * @return
 */
int zebra_client::send_one_oid_request(const QByteArray channel_id, const peersafe::im::OidInfo& oid_info)
{
    if(!channel_id.isEmpty())
    {
        im.send_one_oid_message(channel_id.toStdString(), oid_info, 30, 0);
    }
    else
    {
        MY_DEBUG(2, BROWN)<<"channel_id is empty" << DEBUG_END;

        return -1;
    }

    return 0;
}

/**
 * @brief zebra_client::delete_friend
 * @param friend_name
 * @return
 */
int zebra_client::delete_friend(const QString friend_name)
{
    QVector<DbZebraChannelItem> qvec = ZebraChannelDb::get_instance()->select_zebra_channel_by_chennel_name(friend_name);
    if(qvec.empty())
    {
        MY_DEBUG(2, BROWN)<<"can not find friend = " << friend_name.toStdString() << DEBUG_END;
        return -1;
    }
    else
    {
        foreach(auto it, qvec)
        {
           im.delete_friend(it.channel_id.toStdString(), true);
           ZebraChannelDb::get_instance()->remove_from_zebra_channel(it);

           QMutexLocker locker(&friend_map_lock);
           friend_map.erase(it.channel_id.toStdString());
        }
    }

    return 0;
}

int zebra_client::get_send_cmd_channel(const QString &ip)
{
    /*
    DbPduCommandChannelItem qvec = DbPduCommandChannel::get()->selectItemByIp(ip);
    if(qvec.id < 0)
    {
        return -1;
    }
    else
    {
        int channel = qvec.set_channel;

        if((channel&CHANNEL_ZEBRA) && (channel&CHANNEL_SNMP))
        {
            return 1;//zebra+snmp
        }
        else if((channel&CHANNEL_ZEBRA) && (channel&CHANNEL_CLEVER))
        {
            return 2;//zebra+tcp
        }
        else if(channel&CHANNEL_ZEBRA)
        {
            return 3;//zebra
        }
        else if(channel&CHANNEL_SNMP)
        {
            return 4;//snmp
        }
        else if(channel&CHANNEL_CLEVER)
        {
            return 5;//tcp
        }
        else
        {
            return -1;
        }
    }
    */
    return 3;
}

int zebra_client::send_new_protocol_data(const QString channel_name, uchar *buff, int len)
{
    QVector<DbZebraChannelItem> qvec;

    qvec = ZebraChannelDb::get_instance()->select_zebra_channel_by_chennel_name(channel_name);
    if(qvec.empty())
    {
        MY_DEBUG(2, BROWN) << "can not find friend = " << channel_name.toStdString() << DEBUG_END;

        return -1;
    }

    if(qvec.size() > 1)
    {
        MY_DEBUG(3, LIGHT_PURPLE) << channel_name.toStdString()
                 << " there are " << qvec.size()
                 << " friend channels" << DEBUG_END;
    }

    peersafe::im::OidInfo oid_info;
    oid_info.oid = "";
    oid_info.type = OID_SET;
    oid_info.data_type = 0;

    string data;
    oid_info.value = data.assign((const char *)buff, len);

    send_one_oid_request(qvec[0].channel_id, oid_info);

    return 0;
}

/* 设置设备TRAP通道*/
int zebra_client::set_get_channel_mode(const QString pdu_ip, QString &channel_name)
{
    MY_DEBUG(1, GREEN) << "pdu_ip = " << pdu_ip.toStdString()
             << " channel_name = " << channel_name.toStdString() << DEBUG_END;

    QVector<DbZebraChannelItem> qvec;

    qvec = ZebraChannelDb::get_instance()->select_zebra_channel_by_chennel_name(pdu_ip);
    if(qvec.empty())
    {
        MY_DEBUG(2, YELLOW) << "can not find friend = " << pdu_ip.toStdString() << DEBUG_END;

        return -1;
    }

    if(qvec.size() > 1)
    {
        MY_DEBUG(2, YELLOW) << pdu_ip .toStdString()
                 << " there are " << qvec.size()
                 << " friend channels" << DEBUG_END;
    }

    QString oid = "1.3.6.1.4.1.30966.7.1.1.15";

    peersafe::im::OidInfo oid_info;
    oid_info.oid = oid.toStdString();
    oid_info.type = OID_SET;
    oid_info.value = channel_name.toStdString();
    oid_info.data_type = 1;

    send_one_oid_request(qvec[0].channel_id, oid_info);

    return 0;
}

int zebra_client::send_set_channel_mode(const QString pdu_ip, QString &channel_name)
{
    MY_DEBUG(1, GREEN) << "pdu_ip = " << pdu_ip.toStdString()
             << " channel_name = " << channel_name.toStdString() << DEBUG_END;

    QVector<DbZebraChannelItem> qvec;

    qvec = ZebraChannelDb::get_instance()->select_zebra_channel_by_chennel_name(pdu_ip);
    if(qvec.empty())
    {
        MY_DEBUG(2, YELLOW) << "can not find friend = " << pdu_ip.toStdString() << DEBUG_END;

        return -1;
    }

    if(qvec.size() > 1)
    {
        MY_DEBUG(2, YELLOW) << pdu_ip.toStdString()
                 << " there are " << qvec.size()
                 << " friend channels" << DEBUG_END;
    }

    QString oid = "1.3.6.1.4.1.30966.7.1.1.16";

    peersafe::im::OidInfo oid_info;
    oid_info.oid = oid.toStdString();
    oid_info.type = OID_SET;
    oid_info.value = channel_name.toStdString();
    oid_info.data_type = 1;

    send_one_oid_request(qvec[0].channel_id, oid_info);

    return 0;
}





zebra_cmd_index::zebra_cmd_index()
{
    m_cmd_index = 0;
}

zebra_cmd_index::~zebra_cmd_index()
{

}

zebra_cmd_index *zebra_cmd_index::get_instance()
{
    if(m_cmd_instance == nullptr)
    {
        m_cmd_instance = new zebra_cmd_index();
    }

    return m_cmd_instance;
}

uint8_t zebra_cmd_index::get_cmd_index()
{
    uint8_t now_index = 0;

    QMutexLocker locker(&m_cmd_mutex);
    if(0 == m_cmd_index)
    {
        m_cmd_index++;
    }

    if(m_cmd_index > 127)
    {
        m_cmd_index = 1;
    }

    now_index = m_cmd_index;

    m_cmd_index++;

    return now_index;

}


