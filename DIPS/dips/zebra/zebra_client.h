#ifndef ZEBRA_CLIENT_H
#define ZEBRA_CLIENT_H
#include <QTimer>
#include <QObject>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <QByteArray>
#include <stdint.h>
#include <QMutex>

#include "peersafe/imapi/im.h"
#include "maidsafe/common/active.h"

#define SHORT_CODE_EXPIRE 3660
#define ZEBRA_CONFIG_FLIE "config.ini"

/* 类介绍：
 * 功能：zebra客户端类
 * 注意：无
 */
class zebra_client : public QObject
{
    Q_OBJECT
public :
    static zebra_client *get_instance();

    QString gen_short_code();
    int add_friend(const QString short_code, const QString friend_name);
    int delete_friend(const QString friend_name);
    int send_oids_request(const QByteArray channel_id, const QVector<peersafe::im::OidInfo> qvec_oids);
    int send_one_oid_request(const QByteArray channel_id, const peersafe::im::OidInfo& oid_info);

    /* 获取配置的指令下发通道 */
    int get_send_cmd_channel(const QString &ip);

    /* 发送TCP通道数据协议格式指令 */
    int send_new_protocol_data(const QString channel_name, uchar *buff, int len);

    /* 向ZPDU发送配置的trap通道 */
    int set_get_channel_mode(const QString pdu_ip, QString &channel_name);

    /* 向ZPDU发送配置的指令设置通道 */
    int send_set_channel_mode(const QString pdu_ip, QString &channel_name);

    ~zebra_client();

    void init();

private slots:
    void check_network_state();

private:
    zebra_client();
    void join_network();
    int listen_all_friends();
    void init_friend_map();

    static zebra_client *m_instance;
    QTimer *m_netstate_timer;
    qint64 last_jion_time;
    bool listen_friends_flag;
    bool is_data_process_run;
};

/* 类介绍：
 * 功能：用于生成双因子验证用的指令索引号
 * 注意：无
 */
class zebra_cmd_index : public QObject
{
    Q_OBJECT

public:
    static zebra_cmd_index *get_instance();
    uint8_t get_cmd_index();

    ~zebra_cmd_index();

private:
    zebra_cmd_index();

    static zebra_cmd_index *m_cmd_instance;
    QMutex m_cmd_mutex;
    uint8_t m_cmd_index;

};

#endif // ZEBRA_CLIENT_H
