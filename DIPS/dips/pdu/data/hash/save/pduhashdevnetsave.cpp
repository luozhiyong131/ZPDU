/*
 * pduhashdevnetsave.cpp
 * PDU设备网络信息的保存
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduhashdevnetsave.h"

/**
 * @brief 设备网络地址保存
 * @param ip
 * @param data
 */
static void pdu_hashIPAddr_save(PduNetIPAddr *ip,pdu_dev_data *data)
{
    int fc = data->fn[1] & 0x0f; // 获取高四位
    switch (fc)
    {
    case PDU_CMD_NET_IP: // 设备IP
        pdu_devStr_save(ip->ip, data);
        break;

    case PDU_CMD_NET_GW:
        pdu_devStr_save(ip->gw, data);
        break;

    case PDU_CMD_NET_MASK:
        pdu_devStr_save(ip->mask, data);
        break;

    case PDU_CMD_NET_DNS:
        pdu_devStr_save(ip->dns, data);
        break;

    case PDU_CMD_NET_DNS2:
        pdu_devStr_save(ip->dns2, data);
        break;

    case PDU_CMD_NET_MODE: //网络模式
        ip->mode = data->data[0];
        break;

    default:
        break;
    }

}


/**
 * @brief 设备HTTP信息
 * @param http
 * @param data
 */
static void pdu_hashHttp_save(PduNetHttp *http,pdu_dev_data *data)
{
    int fc = data->fn[1] & 0x0f; // 获取高四位
    switch(fc)
    {
    case PDU_CMD_HTTP_PORT: //设备端口号
        http->port = data->data[0] *256 + data->data[1];
        break;

    case PDU_CMD_SSL_PORT:
        http->ssl = data->data[0] *256 + data->data[1];
        break;

    case PDU_CMD_HTTP_MODE:
        if(data->data[0])
            http->mode = true;
        else
            http->mode = false;
        break;

    default:
        break;
    }
}

/**
 * @brief SNMP信息保存
 * @param snmp
 * @param data
 */
static void pdu_hashSNMP_save(PduNetSNMP *snmp,pdu_dev_data *data)
{
    int fc = data->fn[1] & 0x0f; // 获取高四位
    switch(fc)
    {
    case PDU_CMD_SNMP_EN: // SNMP使能
        if(data->data[0])
            snmp->en = true;
        else
            snmp->en = false;
        break;

    case PDU_CMD_SNMP_GET:
        pdu_devStr_save(snmp->get, data);
        break;

    case PDU_CMD_SNMP_SET:
        pdu_devStr_save(snmp->set, data);
        break;

    case PDU_CMD_SNMP_TRAP1:
        pdu_devStr_save(snmp->trap1, data);
        break;

    case PDU_CMD_SNMP_TRAP2:
        pdu_devStr_save(snmp->trap2, data);
        break;

    case PDU_CMD_SNMP_SERVER:
        pdu_devStr_save(snmp->server, data);
        break;

    case PDU_CMD_SNMP_NODE:
        pdu_devStr_save(snmp->node, data);
        break;

    case PDU_CMD_SNMP_ENV3:
        if(data->data[0])
            snmp->enV3 = true;
        else
            snmp->enV3 = false;
        break;

    case PDU_CMD_SNMP_USR:
        pdu_devStr_save(snmp->usr, data);
        break;

    case PDU_CMD_SNMP_PWD:
        pdu_devStr_save(snmp->pwd, data);
        break;

    case PDU_CMD_SNMP_KEY:
        pdu_devStr_save(snmp->key, data);
        break;

    default:
        break;
    }
}


/**
 * @brief Telenet信息保存
 * @param telnet
 * @param data
 */
static void pdu_hashTelnet_save(PduNetTelnet *telnet,pdu_dev_data *data)
{
    int fc = data->fn[1] & 0x0f; // 获取高四位
    switch(fc)
    {
    case PDU_CMD_TELNET_EN:
        if(data->data[0])
            telnet->en = true;
        else
            telnet->en = false;
        break;

    case PDU_CMD_TELNET_USR:
        pdu_devStr_save(telnet->usr, data);
        break;

    case PDU_CMD_TELNET_PWD:
        pdu_devStr_save(telnet->pwd, data);
        break;

    case PDU_CMD_TELNET_PORT:
        telnet->port = data->data[0] *256 + data->data[1];
        break;

    }
}



/**
 * @brief pdu_hashSmtp_save
 * @param smtp
 * @param data
 */
static void pdu_hashSmtp_save(PduNetSMTP *smtp,pdu_dev_data *data)
{
    int fc = data->fn[1] & 0x0f; // 获取高四位
    switch(fc)
    {
    case PDU_CMD_SMTP_USR:
        pdu_devStr_save(smtp->usr, data);
        break;

    case PDU_CMD_SMTP_PWD:
        pdu_devStr_save(smtp->pwd, data);
        break;

    case PDU_CMD_SMTP_SER:
        pdu_devStr_save(smtp->server, data);
        break;

    case PDU_CMD_SMTP_PORT:
        smtp->port = data->data[0] *256 + data->data[1];
        break;

    case PDU_CMD_SMTP_MODE:
        pdu_devStr_save(smtp->mode, data);
        break;

    case PDU_CMD_SMTP_TEST:
        pdu_devStr_save(smtp->test, data);
        break;
    }
}

/**
 * @brief pdu_hashWifi_save
 * @param wifi
 * @param data
 */
static void pdu_hashWifi_save(PduNetWifi *wifi,pdu_dev_data *data)
{
    int fc = data->fn[1] & 0x0f; // 获取高四位
    switch(fc)
    {
    case PDU_CMD_WIFI_EN:
        if(data->data[0])
            wifi->en = true;
        else
            wifi->en = false;
        break;

    case PDU_CMD_WIFI_SSID:
        pdu_devStr_save(wifi->ssid, data);
        break;

    case PDU_CMD_WIFI_PWD:
        pdu_devStr_save(wifi->pwd, data);
        break;
    }
}

/**
 * @brief SSH登录信息
 * @param ssh
 * @param data
 */
static void pdu_hashSSH_save(PduNetSSH *ssh,pdu_dev_data *data)
{
    int fc = data->fn[1] & 0x0f; // 获取高四位
    switch(fc)
    {
    case PDU_CMD_SSH_EN:
        if(data->data[0])
            ssh->en = true;
        else
            ssh->en = false;
        break;

    case PDU_CMD_SSH_USR:
         pdu_devStr_save(ssh->usr, data);
        break;

    case PDU_CMD_SSH_PWD:
        pdu_devStr_save(ssh->pwd, data);
        break;

    case PDU_CMD_SSH_PORT:
        ssh->port = data->data[0] *256 + data->data[1];
        break;
    }
}

/**
 * @brief Modbus信息
 * @param modbus
 * @param data
 */
static void pdu_hashModbus_save(PduNetModbus *modbus,pdu_dev_data *data)
{
    int fc = data->fn[1] & 0x0f; // 获取高四位
    switch(fc)
    {
    case PDU_CMD_MOD_ADDR:
        modbus->addr = data->data[0];
        break;

    case PDU_CMD_MOD_BAUD:
        modbus->baud = data->data[0] *256 + data->data[1];
        break;

    case PDU_CMD_MOD_DB:
        modbus->db = data->data[0];
        break;

    case PDU_CMD_MOD_PAR:
        modbus->parity = data->data[0];
        break;

    case PDU_CMD_MOD_STOP:
        modbus->stop = data->data[0];
        break;
    }
}

static void pdu_hashNtp_save(PduNetNTP *ntp,pdu_dev_data *data)
{
    int fc = data->fn[1] & 0x0f; // 获取高四位
    switch(fc)
    {
    case PDU_CMD_NTP_EN:
        if(data->data[0])
            ntp->en = true;
        else
            ntp->en = false;
        break;

    case PDU_CMD_NTP_SER:
        pdu_devStr_save(ntp->server, data);
        break;

    case PDU_CMD_NTP_PORT:
        ntp->port = data->data[0] *256 + data->data[1];
        break;

    case PDU_CMD_NTP_ZONE:
        pdu_devStr_save(ntp->timeZone, data);
        break;
    }
}

/**
 * @brief radius相关信息
 * @param radius
 * @param data
 */
static void pdu_hashRadius_save(PduNetRadius *radius,pdu_dev_data *data)
{
    int fc = data->fn[1] & 0x0f; // 获取高四位
    switch(fc)
    {
    case PDU_CMD_RAD_SET:
        if(data->data[0])
            radius->basicSet = true;
        else
            radius->basicSet = false;
        break;

    case PDU_CMD_RAD_EN:
        if(data->data[0])
            radius->radius = true;
        else
            radius->radius = false;
        break;

    case PDU_CMD_RAD_LOCAL:
        if(data->data[0])
            radius->local = true;
        else
            radius->local = false;
        break;

    case PDU_CMD_RAD_SER:
        pdu_devStr_save(radius->serve, data);
        break;

    case PDU_CMD_RAD_KEY:
        pdu_devStr_save(radius->key, data);
        break;

    case PDU_CMD_RAD_AUPORT:
        radius->auPort = data->data[0] *256 + data->data[1];
        break;

    case PDU_CMD_RAD_SERPORT:
        radius->serPort = data->data[0] *256 + data->data[1];
        break;
    }
}



/**
 * @brief 设备用户信息保存
 * @param usr
 * @param data
 */
void pdu_hashDevNet_save(PduNetInfo *net,pdu_dev_data *data)
{
    int fc = data->fn[1] >> 4; // 获取高四位
    switch (fc)
    {
    case PDU_CMD_NET_ADDR: // IPV4 信息
        pdu_hashIPAddr_save(net->ip->ip, data);
        break;

    case PDU_CMD_NET_ADDRV6: // IPV6 信息
        pdu_hashIPAddr_save(net->ip->ip_v6, data);
        break;

    case PDU_CMD_NET_WIFI: // wifi
        pdu_hashWifi_save(net->wifi, data);
        break;

    case PDU_CMD_NET_HTTP: // Http
        pdu_hashHttp_save(net->http, data);
        break;

    case PDU_CMD_NET_SSH:
        pdu_hashSSH_save(net->ssh,data);
        break;

    case PDU_CMD_NET_FTP:
        pdu_hashSSH_save(net->ftp, data);
        break;

    case PDU_CMD_NET_MODBUS:
        pdu_hashModbus_save(net->modbus, data);
        break;

    case PDU_CMD_NET_SNMP: // SNMP
        pdu_hashSNMP_save(net->snmp, data);
        break;

    case PDU_CMD_NET_TELNET: // telnet
        pdu_hashTelnet_save(net->telnet, data);
        break;

    case PDU_CMD_NET_SMTP:
        pdu_hashSmtp_save(net->smtp, data);
        break;

    case PDU_CMD_NET_NTP:
        pdu_hashNtp_save(net->ntp, data);
        break;

    case PDU_CMD_NET_RADIUS:
        pdu_hashRadius_save(net->radius, data);
        break;

    default:
        qDebug() << "pdu_hashDevNet_save err";
        break;
    }

}



