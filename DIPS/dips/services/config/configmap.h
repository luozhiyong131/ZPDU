#ifndef CONFIGMAP_H
#define CONFIGMAP_H
#include "configfile.h"

class ConfigMap
{
    ConfigMap();
public:
     static ConfigMap *bulid();

protected:
     bool readConfigFile();

private:
    devRtuMap mMap;
};

#endif // CONFIGMAP_H
