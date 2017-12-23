#ifndef PDUDEVSETINFO_H
#define PDUDEVSETINFO_H
#include "pdustrbase.h"

class PduDevSetInfo
{
public:
    PduDevSetInfo();

    bool enDelay; /*延时开关*/
    int delay; /*延时时间*/

    bool enBuzzer; /*蜂鸣器开关*/
    bool enGroups; /*分组开关*/
    bool enLcd; /*LCD常高模式*/
};

#endif // PDUDEVSETINFO_H
