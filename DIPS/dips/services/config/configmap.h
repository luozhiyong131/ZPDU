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



};

#endif // CONFIGMAP_H
