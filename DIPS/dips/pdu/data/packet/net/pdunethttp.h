#ifndef PDUNETHTTP_H
#define PDUNETHTTP_H
#include "QtCore"

class PduNetHttp
{
public:
    PduNetHttp();

    int port; //端口
    int ssl; //SSL模式端口
    bool mode; //工作方式
};

#endif // PDUNETHTTP_H
