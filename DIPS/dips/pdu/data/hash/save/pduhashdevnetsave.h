#ifndef PDUHASHDEVNETSAVE_H
#define PDUHASHDEVNETSAVE_H
#include "pduhashcom.h"

// IP地址结构体
typedef enum {
    PDU_CMD_NET_IP=1,
    PDU_CMD_NET_GW,
    PDU_CMD_NET_MASK,
    PDU_CMD_NET_DNS,
    PDU_CMD_NET_DNS2,
    PDU_CMD_NET_MODE,
}PDU_CMD_DEV_NET_IP;

// http 相关
typedef enum {
    PDU_CMD_HTTP_PORT=1, //http端口号
    PDU_CMD_SSL_PORT,
    PDU_CMD_HTTP_MODE,
}PDU_CMD_DEV_NET_HTTP;

// SNMP 相关
typedef enum {
    PDU_CMD_SNMP_EN=1,
    PDU_CMD_SNMP_GET,
    PDU_CMD_SNMP_SET,

    PDU_CMD_SNMP_TRAP1,
    PDU_CMD_SNMP_TRAP2,

    PDU_CMD_SNMP_SERVER,
    PDU_CMD_SNMP_NODE,

    PDU_CMD_SNMP_ENV3,
    PDU_CMD_SNMP_USR,
    PDU_CMD_SNMP_PWD,
    PDU_CMD_SNMP_KEY,
}PDU_CMD_DEV_NET_SNMP;

// Telnet相关
typedef enum {
    PDU_CMD_TELNET_EN=1,
    PDU_CMD_TELNET_USR,
    PDU_CMD_TELNET_PWD,
    PDU_CMD_TELNET_PORT,
}PDU_CMD_DEV_NET_TELNET;


// SMTP 相关
typedef enum {
    PDU_CMD_SMTP_USR=1,
    PDU_CMD_SMTP_PWD,
    PDU_CMD_SMTP_SER,
    PDU_CMD_SMTP_PORT,
    PDU_CMD_SMTP_MODE,
    PDU_CMD_SMTP_TEST,
}PDU_CMD_DEV_NET_SMTP;

// WIFI相关
typedef enum {
    PDU_CMD_WIFI_EN=1,
    PDU_CMD_WIFI_SSID,
    PDU_CMD_WIFI_PWD,
}PDU_CMD_DEV_NET_WIFI;

// SSH相关
typedef enum {
    PDU_CMD_SSH_EN=1,
    PDU_CMD_SSH_USR,
    PDU_CMD_SSH_PWD,
    PDU_CMD_SSH_PORT,
}PDU_CMD_DEV_NET_SSH;

// modbus相关
typedef enum {
    PDU_CMD_MOD_ADDR=1,
    PDU_CMD_MOD_BAUD,
    PDU_CMD_MOD_DB,
    PDU_CMD_MOD_PAR,
    PDU_CMD_MOD_STOP,
}PDU_CMD_DEV_NET_MODBUS;

// NTP 相关
typedef enum {
    PDU_CMD_NTP_EN=1,
    PDU_CMD_NTP_SER,
    PDU_CMD_NTP_PORT,
    PDU_CMD_NTP_ZONE,
}PDU_CMD_DEV_NET_NTP;

// radius 相关
typedef enum {
    PDU_CMD_RAD_SET=1,
    PDU_CMD_RAD_EN,
    PDU_CMD_RAD_LOCAL,

    PDU_CMD_RAD_SER,
    PDU_CMD_RAD_KEY,

    PDU_CMD_RAD_AUPORT,
    PDU_CMD_RAD_SERPORT

}PDU_CMD_DEV_NET_RADIUS;

typedef enum {
    PDU_CMD_NET_ADDR=1, //IPV4信息
    PDU_CMD_NET_ADDRV6,  // IPV6信息

    PDU_CMD_NET_WIFI,
    PDU_CMD_NET_HTTP, //Http功能

    PDU_CMD_NET_SSH,
    PDU_CMD_NET_FTP,

    PDU_CMD_NET_MODBUS,
    PDU_CMD_NET_SNMP, // snmp相关

    PDU_CMD_NET_TELNET, // Telent 相关
    PDU_CMD_NET_SMTP,

    PDU_CMD_NET_NTP,
    PDU_CMD_NET_RADIUS,

}PDU_CMD_DEV_NET_ADDR;


typedef enum {
    PDU_CMD_DEVNET = 7, //设备网络信息 主功能码为7
}PDU_CMD_DEV_NET;

void pdu_hashDevNet_save(PduNetInfo *net,pdu_dev_data *data);
extern void pdu_devStr_save(PduStrBase *name, pdu_dev_data *data);

#endif // PDUHASHDEVNETSAVE_H
