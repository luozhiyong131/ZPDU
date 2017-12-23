#ifndef PDU_ALARMDEV_H
#define PDU_ALARMDEV_H

#include <QObject>
#include <QtCore>
#include "pdu_clearflag.h"
#include "pdu_alarmrecord.h"
//#include"pdu_alarmusic.h"

typedef QMultiHash<QString,int> alarmDevHash;

class PDU_AlarmDev : public QThread
{
    Q_OBJECT
public:
    explicit PDU_AlarmDev(QObject *parent = 0);
    ~PDU_AlarmDev();



protected:
    void run(void);
    int listIP(QStringList &ip);
    int listDev(QString &ip,QList<int> &devList);

    void alarmCheck(void);
    void alarmDev(void);

signals:

protected slots:
    void initFunction(void);
    void timeoutDone(void);

private:
    bool isRun, isOpen;
    QReadWriteLock *mLock;
    alarmDevHash *mHash;

//    Pdu_Alarmusic *mAlarmmusic;
//    QString path;

    QTimer *mTimer;
    PDU_AlarmRecord *mAlarmRecord;


};
void pdu_alarmDev_append(const QString &ip, int num);
  alarmDevHash *getAlarmHash();

#endif // PDU_ALARMDEV_H
