#ifndef UDPHEARTBEAT_H
#define UDPHEARTBEAT_H

#include "udpbdsent.h"

class UdpHeartBeat : public QThread
{
    Q_OBJECT
public:
    explicit UdpHeartBeat(QObject *parent = 0);
    ~UdpHeartBeat();

    void startSent(void);

protected:
    void run(void);
     void heartbeatPacket(void);

signals:

public slots:
    void timeoutDone(void);

private:
    bool isRun;
    UdpSentSocket *mSocket;
    QTimer *timer;

    UdpBDSent *mUdpBDSent;
};

#endif // UDPHEARTBEAT_H
