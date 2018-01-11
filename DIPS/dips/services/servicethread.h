#ifndef SERVICETHREAD_H
#define SERVICETHREAD_H

#include "json/buildjson.h"

class ServiceThread : public QThread
{
    Q_OBJECT
public:
    explicit ServiceThread(QObject *parent = nullptr);
    ~ServiceThread();
    static ServiceThread *bulid();

protected:
    void run();
    void readDevList();

protected slots:

private:
    BuildJson *mBuildJson;
    bool isRun;
};

#endif // SERVICETHREAD_H
