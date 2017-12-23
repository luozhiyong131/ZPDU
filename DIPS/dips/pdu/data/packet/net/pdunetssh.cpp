#include "pdunetssh.h"

PduNetSSH::PduNetSSH()
{
    en = true;
    usr = new PduStrBase();
    pwd = new PduStrBase();
    port = 22;
}

PduNetSSH::~PduNetSSH()
{
    delete usr;
    delete pwd;
}
