#ifndef AUTOADDFRIEND_H
#define AUTOADDFRIEND_H

#include <QVariant>
#include <QDebug>
#include <QTcpSocket>
#include <QTimer>
#include <QMutex>

#include <stdint.h>
#include <list>

const int32_t c_short_code_sock_port = 21010;

class ShortCodeSocket : public QObject
{
    Q_OBJECT

public:
    bool distory_flag;

    ShortCodeSocket(QString zpdu_ip);
    ~ShortCodeSocket();

private slots:
    void read_message();

private:
    QString m_zpdu_ip;
    QTcpSocket* m_shortcode_socket;

};

class AutoAddFriend : public QObject
{
    Q_OBJECT

public:
    AutoAddFriend();
    ~AutoAddFriend();
    static AutoAddFriend* get_instance();
    int addFriend(const QString &ip);

private slots:
    void timeout_to_free_socket();

private:
    static AutoAddFriend* m_auto_addfiend;
    QMutex m_mutex;
    QTimer* m_shortcode_timer;
    std::list<ShortCodeSocket*> m_shordcode_sock;

    bool check_exist(const QString &ip);

};



#endif //AUTOADDFRIEND_H


