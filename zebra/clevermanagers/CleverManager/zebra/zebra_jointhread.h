#ifndef ZEBRA_JOINTHREAD_H
#define ZEBRA_JOINTHREAD_H

#include "zebra_im.h"

class Zebra_JoinThread : public QThread
{
    Q_OBJECT
public:
    explicit Zebra_JoinThread(QObject *parent = nullptr);
    ~Zebra_JoinThread();

protected:
    void run();
    void delay(int s);
    void workDown();

private:
    Zebra_DataPacket *mZebraData;
    bool isRun;
};

#endif // ZEBRA_JOINTHREAD_H
