#ifndef SERVICETHREAD_H
#define SERVICETHREAD_H

#include "json/buildjson.h"
#include "websocket/websocketclient.h"

class ServiceThread : public QThread
{
    Q_OBJECT
public:
    explicit ServiceThread(QObject *parent = nullptr);
    ~ServiceThread();
    static ServiceThread *bulid();

protected:
    void run();
    void readFile();
    void readDevList();

protected slots:

private:
    BuildJson *mBuildJson;
    WebSocketClient *mWebSocket;
    int mSec;
    bool isRun;
};

#endif // SERVICETHREAD_H
