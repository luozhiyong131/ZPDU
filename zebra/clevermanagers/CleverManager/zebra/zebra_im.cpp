#include "zebra_im.h"

Zebra_Im::Zebra_Im(Zebra_Client *c) : im(c)
{
    isRun = false;
}

Zebra_Im::~Zebra_Im()
{
    stopZebra();
}

Zebra_Im *Zebra_Im::bulid(Zebra_Client *c)
{
    static Zebra_Im* sington = nullptr;
    if(sington == nullptr) {
        sington = new Zebra_Im(c);
    }

    return sington;
}

bool Zebra_Im::startZebra()
{
    if(!isRun)
    {
        start();
        isRun = true;
    }

    return isRun;
}


void Zebra_Im::stopZebra()
{
    if(isRun) {
        stop();
        isRun = false;
    }
}


int Zebra_Im::send(const QString &id, uchar *buf, int len)
{
    int ret = 0;
    if(isRun) {
        std::string data((char*)buf,len);
        ret = send_message(id.toStdString().c_str(), data);
    }

    return ret;
}
