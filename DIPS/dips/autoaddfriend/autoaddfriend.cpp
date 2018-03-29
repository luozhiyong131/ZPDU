#include "autoaddfriend.h"

#include <atomic>

#include "zebra/zebra_client.h"
#include "zebra/common_debug.h"
#include "zebra/zebrdatasql.h"

extern std::atomic_int zebra_network_state;

AutoAddFriend* AutoAddFriend::m_auto_addfiend = nullptr;



ShortCodeSocket::ShortCodeSocket(QString zpdu_ip) :
    distory_flag(false)
{
    m_zpdu_ip = zpdu_ip;

    m_shortcode_socket = new QTcpSocket();

    m_shortcode_socket->connectToHost(m_zpdu_ip, c_short_code_sock_port);
    connect(m_shortcode_socket,
            SIGNAL(readyRead()),
            this,
            SLOT(read_message()));
}

ShortCodeSocket::~ShortCodeSocket()
{
    delete m_shortcode_socket;
}

/********************************************************************************
* 函数功能：接收ZPDU发送过来的短码，自动加好友
* 入参说明：无
* 出参说明：无
* 返回值:
* 修改日期：2018-03-15
* 修改目的：功能需求新增
* 修改作者：zjm
********************************************************************************/
void ShortCodeSocket::read_message()
{
    QByteArray buffer;

    buffer = m_shortcode_socket->readAll();
    if(!buffer.isEmpty())
    {
        QString short_code = QString(buffer);
        if(6 == short_code.size())
        {
            MY_DEBUG(1, GREEN) << "short code : " << short_code.toStdString() << DEBUG_END;
            distory_flag = true;
            zebra_client::get_instance()->add_friend(short_code, m_zpdu_ip);
        }
        else
        {
            MY_DEBUG(2, YELLOW) << "short code : "
                                << short_code.toStdString()
                                << " error!"
                                << DEBUG_END;
            distory_flag = false;
        }
    }

    return ;
}

AutoAddFriend::AutoAddFriend()
{
    m_shortcode_timer = new QTimer(this);
    m_shortcode_timer->setSingleShot(false);
    QObject::connect(m_shortcode_timer,
                     SIGNAL(timeout()),
                     this,
                     SLOT(timeout_to_free_socket()));
    m_shortcode_timer->setInterval(3000);
    m_shortcode_timer->start();

}

AutoAddFriend::~AutoAddFriend()
{
    delete m_shortcode_timer;

    QMutexLocker locker(&m_mutex);
    if(m_shordcode_sock.size() != 0)
    {
        std::list<ShortCodeSocket*>::iterator it;
        for(it = m_shordcode_sock.begin(); it != m_shordcode_sock.end(); )
        {
            delete (*it);
            m_shordcode_sock.erase(it++);
        }
    }
}

AutoAddFriend *AutoAddFriend::get_instance()
{
    if(nullptr == m_auto_addfiend)
    {
        m_auto_addfiend = new AutoAddFriend();
    }
	
    return m_auto_addfiend;
}

/********************************************************************************
* 函数功能：提供给外部的接口，用于自动添加ZPDU为好友
* 入参说明：ip--ZPDU的IP地址
* 出参说明：无
* 返回值:
* 修改日期：2018-03-15
* 修改目的：功能需求新增
* 修改作者：zjm
********************************************************************************/
int AutoAddFriend::addFriend(const QString &ip)
{
    bool ret = 0;

    if(zebra_network_state < 4)
    {
        return 1;
    }

    ret = check_exist(ip);
    if (0 == ret)
    {
        ShortCodeSocket* shord_code_tmp = new ShortCodeSocket(ip);
        QMutexLocker locker(&m_mutex);
        m_shordcode_sock.push_back(shord_code_tmp);
    }

    return 0;
}

/********************************************************************************
* 函数功能：检查目的IP设备是否已经在好友列表中
* 入参说明：ip--ZPDU的IP地址
* 出参说明：无
* 返回值:0--不在好友列表中      1--在好友列表中
* 修改日期：2018-03-15
* 修改目的：功能需求新增
* 修改作者：zjm
********************************************************************************/
bool AutoAddFriend::check_exist(const QString &ip)
{
    QVector<DbZebraChannelItem> qvec = ZebraChannelDb::get_instance()->select_zebra_channel_by_chennel_name(ip);

    if(qvec.size())
    {
        MY_DEBUG(2, YELLOW) << ip.toStdString()
                            << " friend exist"
                            << DEBUG_END;

        return 1;
    }
    else
    {
        MY_DEBUG(1, GREEN) << ip.toStdString()
                           << " friend not exist, need to add"
                           << DEBUG_END;;

        return 0;
    }
}

/********************************************************************************
* 函数功能：定时器触发函数，用于释放获取短码的对象列表中已经完成的对象
* 入参说明：无
* 出参说明：无
* 返回值:
* 修改日期：2018-03-15
* 修改目的：功能需求新增
* 修改作者：zjm
********************************************************************************/
void AutoAddFriend::timeout_to_free_socket()
{
    QMutexLocker locker(&m_mutex);
    if(m_shordcode_sock.size() != 0)
    {
        std::list<ShortCodeSocket*>::iterator it;
        for(it = m_shordcode_sock.begin(); it != m_shordcode_sock.end(); )
        {
            if(true == (*it)->distory_flag)
            {
                delete (*it);
                m_shordcode_sock.erase(it++);
            }
            else
            {
                it++;
            }
        }
    }

    return ;
}
