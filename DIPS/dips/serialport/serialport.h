#ifndef SERIALPORT_H
#define SERIALPORT_H
#include <QtCore>

#define SERIAL_BAUDRATE    "9600"

struct Serial_PortData
{
    Serial_PortData():dataBit("8"),parity("none"),stopBit("1"){}
    QString devName;
    QString baudRate;
    QString dataBit;
    QString parity;
    QString stopBit;
    int timeOut;
};

class SerialPort
{
public:
    SerialPort();
    ~SerialPort();

    bool openSerial(const QString &name, const QString &baud=SERIAL_BAUDRATE, int timeOut=0);
    bool setBaudRate(const QString &baud);
    void closeSerial();

    int sentData(uchar *pBuff, int nCount);
    int readData(uchar *pBuf, int nCount);
    int transmit(uchar *sent, int len, uchar *recv);

    bool loopTest();

protected:
    bool openCom(Serial_PortData &);
    int recvData(uchar *pBuf, int nCount);

private:
    int fd; //串口句柄
    QMutex mutex; // 互拆锁
    Serial_PortData mSerial;
};

#endif // SERIALPORT_H
