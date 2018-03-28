#include "zebrasecuritymanager.h"
#include "zebra/common_debug.h"
#include "zebra/signaldispatch.h"


zebraSecurityManager* zebraSecurityManager::m_zebraSecurityManager = nullptr;
zebraSecurityTable* zebraSecurityTable::m_table_instance = nullptr;

const quint16 ZEBRA_SECURITY_SERVER_PORT = 21011;
const uint32_t ZEBRA_SECURITY_MAX_NUM = 200;
const int32_t ZEBRA_SECURITY_TIME = 30*1000;



zebraSecurityTable::zebraSecurityTable()
{
    m_chack_timer = new QTimer(this);
    m_chack_timer->setSingleShot(false);
    QObject::connect(m_chack_timer,
                     SIGNAL(timeout()),
                     this,
                     SLOT(on_timeout_process()));
    m_chack_timer->setInterval(ZEBRA_SECURITY_TIME);
    m_chack_timer->start();
}

zebraSecurityTable::~zebraSecurityTable()
{
    delete m_chack_timer;
}

zebraSecurityTable* zebraSecurityTable::get_instance()
{
    if(nullptr == m_table_instance)
    {
        m_table_instance = new zebraSecurityTable;
    }

    return m_table_instance;
}

void zebraSecurityTable::on_timeout_process()
{
    QMutexLocker locker(&m_mutex);
    std::map<QString, zebra_security_data>::iterator it;
    for(it = m_zebra_security_map.begin(); it != m_zebra_security_map.end(); it++)
    {

        if(it->second.beat_beriod < -(ZEBRA_SECURITY_TIME/1000)*3)
        {
            //nothing to do
            //此处不减，防止无限减导致溢出
        }
        else
        {
            it->second.beat_beriod -= ZEBRA_SECURITY_TIME/1000;
        }
    }

    //TODO：发送更新信号
    SignalDispatch::get()->update_zebra_security_list(m_zebra_security_map);

    return ;
}

int zebraSecurityTable::add_one_zebra_security(char *data_str)
{
    zebra_security_info* data_info = (zebra_security_info*)data_str;
    uint32_t mac_1_c = (uchar)data_info->dev_mac[0];
    uint32_t mac_2_c = (uchar)data_info->dev_mac[1];
    uint32_t mac_3_c = (uchar)data_info->dev_mac[2];
    uint32_t mac_4_c = (uchar)data_info->dev_mac[3];
    uint32_t mac_5_c = (uchar)data_info->dev_mac[4];
    uint32_t mac_6_c = (uchar)data_info->dev_mac[5];
    uint32_t mac_7_c = (uchar)data_info->dev_mac[6];
    uint32_t mac_8_c = (uchar)data_info->dev_mac[7];
    QString device_mac = QString::number(mac_1_c, 16) + "-" +
                         QString::number(mac_2_c, 16) + "-" +
                         QString::number(mac_3_c, 16) + "-" +
                         QString::number(mac_4_c, 16) + "-" +
                         QString::number(mac_5_c, 16) + "-" +
                         QString::number(mac_6_c, 16) + "-" +
                         QString::number(mac_7_c, 16) + "-" +
                         QString::number(mac_8_c, 16);

    QMutexLocker locker(&m_mutex);

    std::map<QString, zebra_security_data>::iterator it = m_zebra_security_map.find(device_mac);
    if(it != m_zebra_security_map.end())
    {
        /*MY_DEBUG(1, RED) << "update"
                         << " beat_beriod = " << it->second.beat_beriod
                         << " dev_ip = " << it->second.dev_ip.toStdString()
                         << " dev_mac = " << device_mac.toStdString()
                         << DEBUG_END;*/
        it->second.beat_beriod = data_info->beat_beriod;

        if(0 != (it->second.dev_ip).compare(QString(QLatin1String(data_info->dev_ip))))
        {
            it->second.dev_ip = QString(QLatin1String(data_info->dev_ip));
            //TODO：发送更新信号
            SignalDispatch::get()->update_zebra_security_list(m_zebra_security_map);
        }

        return 1;
    }
    else
    {
        zebra_security_data data_tmp;
        data_tmp.beat_beriod = data_info->beat_beriod;
        data_tmp.dev_ip = QString(QLatin1String(data_info->dev_ip));
        m_zebra_security_map.insert(std::pair<QString, zebra_security_data>(device_mac, data_tmp));

        //TODO：发送更新信号
        SignalDispatch::get()->update_zebra_security_list(m_zebra_security_map);

        return 0;

    }
}

void zebraSecurityTable::get_table_now()
{
    SignalDispatch::get()->update_zebra_security_list(m_zebra_security_map);
}

void zebraSecurityTable::delete_all_offline_zebra_security()
{
    QMutexLocker locker(&m_mutex);
    std::map<QString, zebra_security_data>::iterator it;
    for(it = m_zebra_security_map.begin(); it != m_zebra_security_map.end(); )
    {
        if(it->second.beat_beriod <= 0)
        {
            m_zebra_security_map.erase(it++);
        }
        else
        {
            it++;
        }
    }

    //TODO：发送更新信号
    SignalDispatch::get()->update_zebra_security_list(m_zebra_security_map);

    return ;
}



zebraSecuritySocket::zebraSecuritySocket(QObject *parent)
{

}

zebraSecuritySocket::~zebraSecuritySocket()
{
    requestInterruption();
    wait();
    quit();
}

void zebraSecuritySocket::init(QTcpSocket *socket)
{
    if(nullptr == socket)
    {
        return ;
    }

    finish_flag = false;

    m_tcp_socket = socket;
    connect(m_tcp_socket,
            SIGNAL(readyRead()),
            this,
            SLOT(readMessage()));

    connect(m_tcp_socket,
            SIGNAL(disconnected()),
            this,
            SLOT(finish_zebra_security_thread()));

    connect(this,
            SIGNAL(reply_zebra_security_sig()),
            this,
            SLOT(reply_zebra_security()));

    return ;
}

void zebraSecuritySocket::run()
{
    while(!isInterruptionRequested())
    {
        requestInterruption();

        if(finish_flag)
        {
            finish_flag = false;

            emit reply_zebra_security_sig();


        }

        msleep(500);
    }
}

void zebraSecuritySocket::readMessage()
{
    QByteArray data_arr = m_tcp_socket->readAll();
    zebraSecurityTable::get_instance()->add_one_zebra_security(data_arr.data());

    finish_flag = true;

    return ;
}

void zebraSecuritySocket::reply_zebra_security()
{
    if(nullptr == m_tcp_socket)
    {
        if(m_tcp_socket->state() == QAbstractSocket::ConnectedState)
        {
            m_tcp_socket->write("ok");

            m_tcp_socket->disconnectFromHost();
            m_tcp_socket->waitForDisconnected();
        }
    }
    else
    {
        MY_DEBUG(2, YELLOW) << "m_tcp_socket is nullptr!" << DEBUG_END;
    }

    return ;
}

void zebraSecuritySocket::finish_zebra_security_thread()
{
    m_tcp_socket->deleteLater();
    requestInterruption();
    quit();
}



zebraSecurityManager::zebraSecurityManager()
    : m_semaphore(ZEBRA_SECURITY_MAX_NUM)
    , m_sock_stroage(ZEBRA_SECURITY_MAX_NUM, nullptr)
{
    m_tcp_server = new QTcpServer(this);

    m_tcp_server->listen(QHostAddress::Any, ZEBRA_SECURITY_SERVER_PORT);

    connect(m_tcp_server,
            SIGNAL(newConnection()),
            this,
            SLOT(new_connect()));

    QVector<zebraSecuritySocket*>::iterator iter = m_sock_stroage.begin();
    for( ; iter!=m_sock_stroage.end(); ++iter)
    {
        *iter = new zebraSecuritySocket();

        connect(*iter,
                SIGNAL(finished()),
                this,
                SLOT(recycleThread()));
    }
}

zebraSecurityManager::~zebraSecurityManager()
{

}

zebraSecurityManager* zebraSecurityManager::get_instance()
{
    if(nullptr == m_zebraSecurityManager)
    {
        m_zebraSecurityManager = new zebraSecurityManager;
    }

    return m_zebraSecurityManager;
}

void zebraSecurityManager::new_connect()
{
    zebraSecuritySocket *sock_thread = nullptr;
    QMutexLocker locker(&m_mutex);

    //MY_DEBUG(1, RED) << "befor use m_semaphore have = " << m_semaphore.available() << DEBUG_END;

    m_semaphore.acquire();
    sock_thread = m_sock_stroage.takeLast();
    if(sock_thread != nullptr)
    {
        sock_thread->init(m_tcp_server->nextPendingConnection());
        sock_thread->start();
    }
}

void zebraSecurityManager::recycleThread()
{
    zebraSecuritySocket *sock_thread = qobject_cast<zebraSecuritySocket*>(sender());
    if(sock_thread)
    {
        QMutexLocker locker(&m_mutex);
        m_sock_stroage.append(sock_thread);
        m_semaphore.release();

        //MY_DEBUG(1, RED) << "after insert m_semaphore have = " << m_semaphore.available() << DEBUG_END;
    }
}













