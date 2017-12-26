#ifndef CONFIGFILE_H
#define CONFIGFILE_H
#include "common.h"




class ConfigFile
{
public:
    ConfigFile();

protected:
    bool writeJson(const QString &name);
    bool readJson(const QString &name);
};

#endif // CONFIGFILE_H
