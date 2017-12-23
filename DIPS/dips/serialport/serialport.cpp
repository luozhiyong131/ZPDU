#include "serialport.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <QObject>
#include <qsocketnotifier.h>
#include <sys/termios.h>
#include <QDebug>
#include <QMutexLocker>


SerialPort::SerialPort()
{
    fd = -1;
}

SerialPort:: ~SerialPort()
{
    closeSerial();
}

bool SerialPort::openCom(Serial_PortData &setData)
{
    struct termios setting;
    fd = open(qPrintable("/dev/" + setData.devName), O_RDWR);
    if (fd == -1) { //串口是否存在
        //QMessageBox::warning(this, "openSerial error", strerror(errno));
        qDebug() << "Serial Err::openSerial "<< setData.devName;
        goto openSerialerr;
    }
    tcgetattr(fd, &setting);

    //  设置波特率
    speed_t speed;
    if (setData.baudRate.contains("4800")) {
        speed = B4800;
    } else if (setData.baudRate.contains("9600")) {
        speed = B9600;
    } else if (setData.baudRate.contains("19200")) {
        speed = B19200;
    } else if (setData.baudRate.contains("38400")) {
        speed = B38400;
    } else if (setData.baudRate.contains("57600")) {
        speed = B57600;
    } else if (setData.baudRate.contains("115200")) {
        speed = B115200;
    } else if (setData.baudRate.contains("230400")) {
        speed = B230400;
    } else if (setData.baudRate.contains("460800")) {
        speed = B460800;
    } else {
        //QMessageBox::warning(this, "error", "speed error!");
        goto seterr;
    }
    cfsetispeed(&setting, speed);
    cfsetospeed(&setting, speed);
    cfmakeraw(&setting);

    //  设置数据位
    if (setData.dataBit == "8") {
        setting .c_cflag |= CS8;
    } else if (setData.dataBit == "7") {
        setting.c_cflag |= CS7;
    } else if (setData.dataBit == "6") {
        setting.c_cflag |= CS6;
    } else if (setData.dataBit == "5") {
        setting.c_cflag |= CS5;
    } else {
        //QMessageBox::warning(this, "error", "dataBit error!");
        goto seterr;
    }

    //  设置parity
    if (setData.parity == "none") {
        setting.c_cflag &= ~PARENB;
        setting.c_iflag &= ~INPCK;
    } else if (setData.parity == "odd") {
        setting.c_cflag |= (PARODD | PARENB);
        setting.c_iflag |= INPCK;
    } else if (setData.parity == "even") {
        setting.c_cflag |= PARENB;
        setting.c_cflag &= ~PARODD;
        setting.c_iflag |= INPCK;
    } else {
        // QMessageBox::warning(this, "error", "dataBit error!");
        goto seterr;
    }

    //  设置停止位
    if (setData.stopBit == "1") {
        setting.c_cflag &= ~CSTOPB;
    } else if (setData.stopBit == "2") {
        setting.c_cflag |= CSTOPB;
    } else {
        //QMessageBox::warning(this, "error", "stopBit error!");
        goto seterr;
    }

    tcflush(fd, TCIFLUSH);
    setting.c_cc[VTIME] = setData.timeOut; // 超时时间 0.1S
    setting.c_cc[VMIN] = 0; // 数据最小长度
    tcsetattr(fd, TCSANOW, &setting);

    //    monitor = new QSocketNotifier(fd, QSocketNotifier::Read, NULL);
    //    connect(monitor, SIGNAL(activated(int)), this, SLOT(readDataSlot()));
    //    monitor->setEnabled(true);

    //   clearData();
    return true;

seterr:
    ::close(fd);

openSerialerr:
    return false;
}

/**
  * 功　能：关闭端口
  * 入口参数：
  * 出口参数：
  * 返回值：
  */
void SerialPort::closeSerial()
{
    if(fd >= 0)
    {
        ::close(fd);
        fd = -1;
    }
}

/**
  * 功　能：打开端口
  * 入口参数：serialName> 设备名
  * 出口参数：
  * 返回值：true
  */
bool SerialPort::openSerial(const QString &name, const QString & baud, int timeOut)
{
    mSerial.devName = name; // 设置串口名
    mSerial.baudRate = baud;
    mSerial.timeOut = timeOut;
    if(fd>=0) closeSerial();
    return openCom(mSerial);
}

bool SerialPort::setBaudRate(const QString &baud)
{
    bool ret = false;

    if(fd>=0) {
        mSerial.baudRate = baud;
        closeSerial();
        ret = openCom(mSerial);
    }

    return ret;
}

/**
  * 功　能：发送数据
  * 入口参数：pBuff -> 缓冲区   nCount -> 长度
  * 返回值：发送的实际长度  <=0 出错
  */
int SerialPort::sentData(uchar *pBuff, int nCount)
{
    if(fd >= 0)
    {
        QMutexLocker locker(&mutex);
        int sendLen = write(fd, pBuff, nCount);
        fsync(fd);
        return sendLen;
    }

    return fd;
}

int SerialPort::readData(uchar *pBuf, int nCount)
{
    int rtn = 0;
    if(fd >= 0)
    {
        QMutexLocker locker(&mutex);
        rtn = read(fd, pBuf, nCount);
    }

    return rtn;
}

/**
  * 功　能：读取数据
  * 入口参数：pBuf -> 缓冲区
  * 返回值：读取的实际长度  <=0 出错
  */
int SerialPort::recvData(uchar *pBuf, int nCount)
{
    int rtn = -1, ret=0;

    do {
        rtn = readData(pBuf, nCount);
        if(rtn > 0) {
            pBuf += rtn; // 指针移动
            ret += rtn; // 长度增加
        } else if(ret >= nCount)
            break;
    } while(rtn>0);

    return ret;
}

/**
  * 功　能：传输数据
  * 入口参数：sent -> 发送缓冲区, len ->  发送长度
  * 出口参数：recv -> 接收缓冲区
  * 返回值：读取的实际长度  <=0 出错
  */
int SerialPort::transmit(uchar *sent, int len, uchar *recv)
{
    int ret = sentData(sent, len);
    if(ret > 0) {
        ret = recvData(recv, 250);
        // if(ret <=0 ) qDebug() << "Serial Trans Err!!!";
    }
    return ret;
}


/**
 * @brief 回环测试
 */
bool SerialPort::loopTest()
{
    uchar sentArray[256], recvArray[512];

    for(int i=0; i<255;++i)
        sentArray[i] = i;

    int ret = transmit(sentArray, 255, recvArray);
    if(ret != 255)
        qDebug() << "Serial test Err:" <<ret;
    else
        qDebug() << "Serial test OK";
    return ret;
}
