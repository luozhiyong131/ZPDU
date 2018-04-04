#ifndef ZEBRASECURITYMANAGER_H
#define ZEBRASECURITYMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSemaphore>
#include <QThread>
#include <QTimer>
#include <QMutex>

#include <stdint.h>

typedef struct zebra_security_info_t
{
    int32_t beat_beriod;
    char dev_ip[16];
    char dev_mac[8];
}zebra_security_info;

typedef struct zebra_security_data_t
{
    int32_t beat_beriod;
    QString dev_ip;
}zebra_security_data;

class zebraSecurityTable : public QObject
{
    Q_OBJECT

public:
    ~zebraSecurityTable();
    static zebraSecurityTable* get_instance();
    int add_one_zebra_security(char *data_str);
    void delete_all_offline_zebra_security();
    void get_table_now();

private slots:
    void on_timeout_process();

private:
    static zebraSecurityTable* m_table_instance;
    QTimer* m_chack_timer;
    QMutex  m_mutex;
    std::map<QString, zebra_security_data> m_zebra_security_map;

    zebraSecurityTable();

};

class zebraSecuritySocket : public QThread
{
    Q_OBJECT

public:
    zebraSecuritySocket(QObject *parent=nullptr);
    ~zebraSecuritySocket();
    void init(QTcpSocket *socket = nullptr);
    void finish_zebra_security_thread();

protected:
    void run();

signals:
    void reply_zebra_security_sig();

private slots:
    void readMessage();
    void reply_zebra_security();


private:
    bool finish_flag;
    QTcpSocket* m_tcp_socket;
};

class zebraSecurityManager : public QTcpServer
{
    Q_OBJECT

public:
    ~zebraSecurityManager();
    static zebraSecurityManager* get_instance();

private slots:
    void new_connect();
    void recycleThread();

private:
    static zebraSecurityManager* m_zebraSecurityManager;
    QTcpServer *m_tcp_server;
    QVector<zebraSecuritySocket*> m_sock_stroage;
    QMutex          m_mutex;
    QSemaphore      m_semaphore;

    zebraSecurityManager();
};

#endif // ZEBRASECURITYMANAGER_H
