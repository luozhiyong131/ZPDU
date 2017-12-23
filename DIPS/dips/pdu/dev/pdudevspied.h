#ifndef PDUDEVSPIED_H
#define PDUDEVSPIED_H

#include "offLine/pdu_offlinedev.h"
#include "devAlarm/pdu_alarmdev.h"

typedef struct
{
    ushort num;
    ushort line;
    ushort off;
    ushort alarm;
}pdu_devNumStr;

class PduDevSpied : public QThread
{
    Q_OBJECT
public:
    explicit PduDevSpied(QObject *parent = 0);
    ~PduDevSpied();

protected:
    void run(void);
    void checkDevState(void);

signals:

public slots:
    void timeoutDone(void);
    void initFunction(void);

private:
    bool isRun;
    QTimer *mTimer;

//    PduDLSaveThread *mDLSave;
    PDU_OFFLineDev *mOffLine;
    PDU_AlarmDev *mAlarmDev;

//    DbSlaveRecordsThread *mDbRecords;
};

pdu_devNumStr *get_pdu_num(void);

#endif // PDUDEVSPIED_H
