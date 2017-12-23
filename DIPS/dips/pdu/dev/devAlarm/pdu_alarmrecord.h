#ifndef PDU_ALARMRECORD_H
#define PDU_ALARMRECORD_H

#include <QObject>
#include "pdu/data/hash/read/pduhashread.h"

typedef struct
{
    QString devIP;
    QString devName;
    QString title;
    QString type;
    QString line;
    QString msg;
}s_AlarmLog;

class PDU_AlarmRecord : public QThread
{
    Q_OBJECT
public:
    explicit PDU_AlarmRecord(QObject *parent = 0);

    void setDev(QString &ip, int num);

protected:
    bool alarmDataUnit(PduDataUnit *unit, s_AlarmLog &log, const QString &);
    bool alarmDataObject(PduObjData *obj, s_AlarmLog &log);

    bool alarmLineData(PduDevData *data);
    bool alarmLoopData(PduDevData *data);
    bool alarmBitData(PduDevData *data);

    bool alarmEnvTH(PduEnvData *envData, s_AlarmLog &log);
    bool alarmDoor(PduEnvData *envData, s_AlarmLog &log);
    bool alarmEnvDataBase(PduDataBase *alarm, PduDataBase *flag, s_AlarmLog &log);
    bool alarmEnv(PduEnvData *envData);

    void alarmCloudLog(s_AlarmLog &log);
    void alarmDev(PduDevData *devData);
    void sentEmail(void);

    void insertAlarm(s_AlarmLog &log);
private:
    void initAlarmLog(s_AlarmLog &log);

signals:
    void alarmSig(QString str);

public slots:
    void alarmSlot(QString str);

private:
    QString smsMsg;
    PduDataPacket *mDataPacket;
    bool isOpen;
};

extern void sim_sent_sms(QString &msg);
extern void email_sent_message(QString &subject, QString &body);
#endif // PDU_ALARMRECORD_H
