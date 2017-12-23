#ifndef PDUNETSSH_H
#define PDUNETSSH_H
#include "pdustrbase.h"

class PduNetSSH
{
public:
    PduNetSSH();
    ~PduNetSSH();

    bool en;
    PduStrBase *usr;
    PduStrBase *pwd;
    int port;
};

#endif // PDUNETSSH_H
