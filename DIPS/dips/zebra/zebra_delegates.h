#ifndef ZEBRA_DELEGATES_H
#define ZEBRA_DELEGATES_H

#include <iostream>
#include <map>
#include <set>

#include "peersafe/imapi/im.h"
#include "maidsafe/common/active.h"
#include "pdu/data/hash/read/pduhashread.h"

#define ICE_CACHE_KEY "ICE_SERVICE_CACHE_KEY"
#define LINUX_DOMAIN  "/tmp/linux.domain"
#define ZEBRA_DATA_THREAD_NUM 50

#define NONE          "\033[m"
#define RED           "\033[0;32;31m"
#define LIGHT_RED     "\033[47;31m"
#define GREEN         "\033[0;32;32m"
#define LIGHT_GREEN   "\033[1;32m"
#define BLUE          "\033[0;32;34m"
#define LIGTH_BLUE    "\033[47;34m"
#define DARY_GRAY     "\033[47;30m"
#define CYAN          "\033[0;36m"
#define LIGHT_CYAN    "\033[1;36m"
#define PURPLE        "\033[0;35m"
#define LIGHT_PURPLE  "\033[1;35m"
#define BROWN         "\033[0;33m"
#define YELLOW        "\033[1;33m"
#define LIGHT_GRAY    "\033[0;37m"
#define WHITE         "\033[1;37m"
#define CLEAN         "\033[2J"

class zebra_delegates :  public QObject, public peersafe::im::Message_delegates
{
    Q_OBJECT

public:
    zebra_delegates();
    ~zebra_delegates();

    void friend_request_via_qr(
            const std::string &qr_channel_id,
            const std::string &info,
            const std::string &friend_channel_id);
    void friend_request_reply(
            const std::string &friend_channel_id,
            bool accepted);

    void friend_deleted(const std::string &friend_channel_id);
    void store_data(const std::string &key_id, const std::string &store_data);
    void read_data(const std::string &key_id, std::string &read_data);
    void delete_data(const std::string &key_id);
    void handler_write_message_reply(
            const std::string &channel_id,
            unsigned long message_id,
            int ret_code);
    void friend_one_oid_message(
        const std::string &friend_channel_id,
        const unsigned long message_id,
        const peersafe::im::OidInfo& oid_info);
    void friend_multi_oid_message(
        const std::string &friend_channel_id,
        const unsigned long message_id,
        const std::vector<peersafe::im::OidInfo> &vecOidInfo);


    //unuseful interface
    void friend_state(const std::string &friend_channel_id, int state_code){

    }
    void network_state(int stat_code){

    }
    void short_code_denied(
            const std::string &qr_channel_id,
            const std::string &short_code){

    }
    void friend_offline_message(
            const std::string &friend_channel_id,
            const int type,
            const std::string &message,
            unsigned long message_id,
            int expired, int entire_expired,
            int length, int timestamp, int device_type){

    }
    void friend_online_message(
            const std::string &friend_channel_id,
            const int type,
            const std::string &message,
            unsigned long message_id,
            int expired, int entire_expired,
            int length, int timestamp, int device_type){

    }
    void ice_session_state(const std::string &channel_id, int state){

    }
    void handle_friend_request_done(
            const std::string &friend_channel_id,
            int ret_code){

    }
    bool isExisted(const std::string &item, unsigned int expire);
    int send_message_via_direct_connection(
            const std::string &connection_id,
            const std::string &friend_channel_id,
            unsigned long message_id,
            const std::string &data) {
        return -1;
    }
    void bind_friend_channel(
            const std::string &connection_id,
            const std::string &friend_channel_id) {

    }
    void group_chat_invite_received(
            const std::string &friend_channel_id,
            const std::string &group_channel_id,
            const std::string &info) {
    }
    void group_chat_member(
            const std::string &group_channel_id,
            const std::string &member_id,
            const std::string &info,
            unsigned long join_time) {
    }
    void group_chat_member_removed(
            const std::string &group_channel_id,
            const std::string &remover,
            const std::string &member_id,
            unsigned long removed_time) {
    }
    void group_chat_message_received(
            const std::string &group_channel_id,
            const std::string &author,
            const int type,
            const std::string &message,
            unsigned long message_id,
            int expired,
            int entire_expired,
            int length,
            int timestamp,
            const std::string &name,
            int device_type) {
    }
    void group_chat_invite_reply_received(
            const std::string &friend_channel_id,
            const std::string &group_channel_id,
            bool accepted,
            const std::string &friend_member_id) {
    }
    void group_channel_name_changed(
            const std::string &group_channel_id,
            const std::string &channel_name,
            unsigned long changed_time) {
    }
    // callback of sending file
    int send_file_result(const int result,
            unsigned long fileid,
            const std::string &channel_id) {
        return 0;
    }
    int recv_file_result(
            const int result,
            const std::string &channel_id,
            const std::string &digest) {
        return 0;
    }
    int handle_file_digest(
            const std::string &channel_id,
            const std::string &digest,
            std::string &path) {
        return 0;
    }
    int handle_send_file_process(
            const std::string &channel_id,
            unsigned long fileid,
            long offset) {
        return 0;
    }
    int handle_recv_file_process(
            const std::string &channel_id,
            const std::string &digest,
            long offset) {
        return 0;
    }

private:
    maidsafe::Active g_io_service;
    std::map<const std::string, std::string> data_store;
};



/*
 * 用于存储ZPDU的trap数据的结构体
 */
struct zebra_oid_data
{
    string channel_id;
    vector<peersafe::im::OidInfo> info;
};

/* 类介绍：
 * 功能：用于保存接收到的PDUtrap数据
 * 注意：处理数据的线程并发
 */
class zebra_data_quene : public QObject
{
    Q_OBJECT

public:
    ~zebra_data_quene();
    static zebra_data_quene *get_instance();
    int get_data_from_quene(struct zebra_oid_data &data);
    int add_data_to_quene(struct zebra_oid_data &data);

private:
    zebra_data_quene();

    static zebra_data_quene *m_instance;
    QReadWriteLock  m_lock;
    std::list<struct zebra_oid_data> data_stroage;

};

/* 类介绍：
 * 功能：用于处理接收到的PDUtrap数据
 * 注意：线程池，目前为ZEBRA_DATA_THREAD_NUM = 50个线程资源
 */
class zebra_data_process_thread : public QThread
{
    Q_OBJECT

public:
    ~zebra_data_process_thread();
    zebra_data_process_thread(QObject *parent=nullptr);

protected:
    void run();

private:

};

class zebra_process_thread_stroage : public QObject
{
    Q_OBJECT

public:
    ~zebra_process_thread_stroage();
    static zebra_process_thread_stroage *get_intance();

private:
    static zebra_process_thread_stroage *m_intance;
    QVector<zebra_data_process_thread *> m_process;

    zebra_process_thread_stroage(int thread_num);

};

/*
 * ZPDU相关组件最大个数的宏
 */
#define ZPDU_OUTPUT_MAX_NUM      24
#define ZPDU_TEMP_SENSOR_MAX_NUM 2
#define ZPDU_HUMI_SENSOR_MAX_NUM 2
#define ZPDU_DOOR_SENSOR_MAX_NUM 2
#define ZPDU_LINE_MAX_NUM        6

#define COM_RATE_VOL	10.0    // 电压
#define COM_RATE_CUR	10.0    // 电流
#define COM_RATE_POW	1000.0  // 功率
#define COM_RATE_ELE	10.0    // 电能
#define COM_RATE_PF		10.0    // 功率因素
#define COM_RATE_TEM	10.0    // 温度
#define COM_RATE_HUM	10.0    // 湿度

/* 结构体数据顺序为PDU传送数据的协议格式
 * 注意：如有改动，请与ZPDU配套修改
 */
typedef struct pdu_trap_data
{
    int  ms;					//主副机
    int  comType;				//zebra 变量，执行下发通道
    int  trap_ch;				//zebra 变量，trap通道
    int  type;					//设备类型
    int  outputnum;				//输出位个数
    char devicename[32];		//设备名称
    char deviceaddr[32];		//设备地址
    char ipaddr[40];			//设备IP地址
    char macaddr[20];			//设备mac地址

    int  temp[2];				//温度，0-1路
    int  hum[2];				//湿度，0-1路
    int  minT[2];				//最小温度，0-1路
    int  minH[2];				//最小湿度，0-1路
    int  maxT[2];				//最大温度，0-1路
    int  maxH[2];				//最大湿度，0-1路
    int  dcritT[2];			    //下临界温度，0-1路
    int  dcritH[2];				//下临界湿度，0-1路
    int  ucritT[2];			    //上临界温度，0-1路
    int  ucritH[2];				//上临界湿度，0-1路
    int  door[2];				//门禁，0-1路
    int  water;					//水浸
    int  smoke;					//烟雾

    int  Tcur[6];				//总电流 6路
    int  Tvol[6];				//总电压 6路
    int  Tkwh[6];				//总电能
    int  minTcur[6];		 	//最小总电流阈值
    int  maxTcur[6];			//最大总电流阈值
    int  minTVol[6];			//最小总电压阈值
    int  maxTVol[6];			//最大总电压阈值
    int  minTCcrit[6];			//最小总电流预警值
    int  maxTCcrit[6];			//最大总电流预警值
    int  minTVcrit[6];			//最小总电压预警值
    int  maxTVcrit[6];			//最大总电压预警值
    int  Tpowerfactor[6];		//总功率因素

    int  cur[24];				//单个输出位电流，zpdu前24位
    int  mincur[24];
    int  maxcur[24];
    int  mincrit[24];
    int  maxcrit[24];
    int  powerfactor[24];
    int  kwh[24];
    int  swicth[24];			//单个输出位开关状态
    char outputname[24][32];	//单个输出位名称

}pdudata;

/* 类介绍：
 * 功能：用于解析接收到的PDUtrap数据
 * 注意：无
 */
class analyze_pdu_trap_data
{
public:
    analyze_pdu_trap_data(QString channel_name,
                          int slave_num,
                          std::string data);
    int get_all_data(QString ip, int slave_num);

    int get_is_master_flag();
    int get_pdu_set_cmd_channel();
    int get_pdu_trap_channel();
    int get_pdu_device_type();
    int get_output_max_bum();
    QString get_device_name();
    QString get_device_addr();
    QString get_device_ip();
    QString get_device_mac();

    int get_pdu_max_temperature();
    int get_pdu_min_temperature();
    int get_pdu_cur_temperature();
    int get_pdu_up_critical_temperature();
    int get_pdu_down_critical_temperature();

    int get_pdu_max_hunidity();
    int get_pdu_min_hunidity();
    int get_pdu_cur_hunidity();
    int get_pdu_up_critical_hunidity();
    int get_pdu_down_critical_hunidity();

    int get_pdu_door_state();
    int get_pdu_smoke_state();
    int get_dpu_water_state();

    int get_total_current();
    int get_total_max_current();
    int get_total_min_current();
    int get_total_up_critical_current();
    int get_total_down_critical_current();

    int get_total_voltage();
    int get_total_max_voltage();
    int get_total_min_voltage();
    int get_total_up_critical_voltage();
    int get_total_down_critical_voltage();

    int get_total_electric_energy();
    int get_pdu_power_factor();

    int get_pdu_output_cur_current();
    int get_pdu_output_min_current();
    int get_pdu_output_max_current();
    int get_pdu_output_max_critical_current();
    int get_pdu_output_min_critical_current();
    int get_pdu_output_power_factor();
    int get_pdu_output_electric_energy();
    int get_pdu_output_switch_state();
    int get_pdu_output_name();

private:
    pdudata m_pdu_data;
    PduDataPacket *m_packet;

};

#endif // ZEBRA_DELEGATES_H


