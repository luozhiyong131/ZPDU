#include "configmap.h"

ConfigMap::ConfigMap()
{

}


ConfigMap *ConfigMap::bulid()
{
    static ConfigMap* sington = nullptr;
    if(sington == nullptr)
        sington = new ConfigMap();
    return sington;
}

bool ConfigMap::readConfigFile()
{

}
