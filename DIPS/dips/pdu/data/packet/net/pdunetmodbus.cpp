#include "pdunetmodbus.h"

PduNetModbus::PduNetModbus()
{
    addr = 0; //无效
    baud = 9600; //波特
    db = 8; //数据位
    parity = 0 ; //奇偶性  无
    stop = 1; //停止位
}

