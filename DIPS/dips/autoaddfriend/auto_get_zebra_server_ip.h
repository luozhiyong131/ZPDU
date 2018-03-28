#ifndef AUTO_GET_ZEBRA_SERVER_IP_H
#define AUTO_GET_ZEBRA_SERVER_IP_H



#include <QThread>
#include <QtNetwork>
#include <QObject>
#include <QtCore>


#define BROADCAST_PORT_NUM  20100


/***********************************************************************************
* Function describe：自动获取zebra security的IP地址
* Matters attention：
* Input parameter：
* Output parameter：
* Author：zjm : )
* Creation time：
* Modify log：zjm => 2018-03-27 => add describe
***********************************************************************************/
class AutoGetServer : public QObject
{
    Q_OBJECT

public:
    explicit AutoGetServer(QObject *parent = 0);
    ~AutoGetServer();
    static AutoGetServer* get_instance();

public slots:
    void readMessageSlot();

protected:
    bool finish_flag;
    static AutoGetServer* m_auto_get_server;
    QUdpSocket udpSocket;

};



#endif // AUTO_GET_ZEBRA_SERVER_IP_H


