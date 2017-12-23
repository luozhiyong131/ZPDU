#ifndef UDPSOCKETLIST_H
#define UDPSOCKETLIST_H

#include "udpdataque.h"
#include "udpsocketport.h"

#define UDP_PORT_SIZE   20 /*UDP监听端口最大个数*/

class UdpSocketList : public QThread
{
    Q_OBJECT
public:
    explicit UdpSocketList(QObject *parent = 0);
    ~UdpSocketList();

     void initSocket(int);
    void stopAndWaiting();
protected:
    void run(void);
    int recvData(void);

signals:

public slots:

private:
    bool isRun;
    UdpRecvSocket *mSocket;
    UdpBaseData *mData;
    QList<UdpRecvSocket *> *mSocketList;
};

#endif // UDPSOCKETLIST_H
