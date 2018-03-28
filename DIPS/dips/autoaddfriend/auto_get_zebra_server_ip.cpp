#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string>
#include <errno.h>
#include <thread>
#include <stdint.h>
#include <QDebug>
#include <QString>

#include "auto_get_zebra_server_ip.h"
#include "zebra/common_debug.h"



QString auto_zebra_security_ip = "";

AutoGetServer* AutoGetServer::m_auto_get_server = nullptr;


AutoGetServer::AutoGetServer(QObject *parent) :
    QObject(parent),
    finish_flag(false)
{
    udpSocket.bind(BROADCAST_PORT_NUM, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(&udpSocket, SIGNAL(readyRead()), this, SLOT(readMessageSlot()));
}

AutoGetServer::~AutoGetServer()
{

}

AutoGetServer *AutoGetServer::get_instance()
{
    if(nullptr == m_auto_get_server)
    {
        m_auto_get_server = new AutoGetServer;
    }

    return m_auto_get_server;
}

void AutoGetServer::readMessageSlot()
{
    QByteArray datagram;
    QString qstr_msg;

    if(!finish_flag)
    {
        if(udpSocket.hasPendingDatagrams())
        {
            datagram.resize(udpSocket.pendingDatagramSize());
            udpSocket.readDatagram(datagram.data(), datagram.size());
            qstr_msg = datagram.data();
            qstr_msg = qstr_msg.remove(QString("\n"));
            QString str_ip_regexp ="((([1-9][0-9]?)|(1[0-9]?[0-9]?)|(2[0-4]?[0-9])|(25?[0-5]))\\.){1}"
                                  "((0|([1-9][0-9]?)|(1[0-9]?[0-9]?)|(2[0-4]?[0-9])|(25?[0-5]))\\.){2}"
                                  "(0|([1-9][0-9]?)|(1[0-9]?[0-9]?)|(2[0-4]?[0-9])|(25?[0-5])){1}";
            QRegExp rx(str_ip_regexp);
            if(rx.exactMatch(qstr_msg))
            {
                auto_zebra_security_ip = qstr_msg;
                finish_flag = true;
                MY_DEBUG(1, GREEN) << "auto get zebra server IP = " << qstr_msg.toStdString() << DEBUG_END;
            }
            else
            {
                MY_DEBUG(2, YELLOW) << "get zebra server IP is empty!" << DEBUG_END;
            }
        }
    }
}


