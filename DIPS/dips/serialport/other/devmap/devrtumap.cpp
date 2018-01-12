#include "devrtumap.h"

DevRtuMap::DevRtuMap()
{
    readFile();
}


DevRtuMap *DevRtuMap::bulid()
{
    static DevRtuMap* sington = nullptr;
    if(sington == nullptr)
        sington = new DevRtuMap();
    return sington;
}

bool DevRtuMap::readFile()
{
    DevMapFile file;
    return file.readJson(mMap);
}

bool DevRtuMap::get(int devNum, sMapItem &item)
{
    bool ret = mMap.contains(devNum);
    if(ret) {
        item = mMap.value(devNum);
    }
    return ret;
}

int DevRtuMap::list(QList<int> &list)
{
    list = mMap.keys();;
    return size();
}
