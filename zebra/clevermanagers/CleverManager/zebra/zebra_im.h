#ifndef ZEBRA_IM_H
#define ZEBRA_IM_H

#include "zebra_client.h"

class Zebra_Im : public im
{
    Zebra_Im(Zebra_Client *c);
public:
    ~Zebra_Im();
    static Zebra_Im *bulid(Zebra_Client *c = nullptr);

    bool startZebra();
    void stopZebra();
    int send(const QString &id, uchar *buf, int len);

private:
    bool isRun;
};

#endif // ZEBRA_IM_H
