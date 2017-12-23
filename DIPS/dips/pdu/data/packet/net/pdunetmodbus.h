#ifndef PDUNETMODBUS_H
#define PDUNETMODBUS_H


class PduNetModbus
{
public:
    PduNetModbus();

    int addr;
    int baud; //波特
    int db; //数据位
    int parity; //奇偶性
    int stop; //停止位
};

#endif // PDUNETMODBUS_H
