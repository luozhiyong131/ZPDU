#ifndef PDU_OFFLINEDEV_H
#define PDU_OFFLINEDEV_H

#include <QtCore>
#include <QObject>
#include "pdu/data/hash/read/pduhashread.h"

typedef QMultiHash<QString,int> offLineHash;

typedef struct
{
    QString devIP;
    QString devName;
    QString title;
    QString msg;
}sOffLineLog;

class PDU_OFFLineDev : public QThread
{
    Q_OBJECT
public:
    explicit PDU_OFFLineDev(QObject *parent = 0);
    ~PDU_OFFLineDev();

protected:
    void run(void);
    void offLineCheck(void);
    void offLineDev(void);

    int listIP(QStringList &ip);
    int listDev(QString &ip,QList<int> &devList);
    void offLineDevLog(QString &ip, int num);
    void offLineCloudLog(PduDataPacket *dataPacket);

signals:
    void offLineSig(QString, QString, QString);

protected slots:
    void initFunction(void);
    void timeoutDone(void);
    void offLineDevSlot(QString, QString, QString);

private:
    bool isRun, isOpen;
    QReadWriteLock *mLock;
    offLineHash *mHash;

    QTimer *mTimer;
};

void off_line_append(const QString &ip, int num);

extern void sim_sent_sms(QString &msg);
extern void email_sent_message(QString &subject, QString &body);

#endif // PDU_OFFLINEDEV_H
