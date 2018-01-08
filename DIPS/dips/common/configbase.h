#ifndef CONFIGBASE_H
#define CONFIGBASE_H
#include "sysconfigfile.h"

class ConfigBase
{
public:
    ConfigBase();

    QString getSerialName();
    void setSerialName(const QString &name);

    QString getBaudRate();
    void setBaudRate(const QString &rate);

    virtual QString getPrefix(){return "system";}
};

#endif // CONFIGBASE_H
