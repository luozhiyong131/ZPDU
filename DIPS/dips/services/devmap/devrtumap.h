#ifndef DEVRTUMAP_H
#define DEVRTUMAP_H
#include "devmapfile.h"

class DevRtuMap
{
    DevRtuMap();
public:
    static DevRtuMap *bulid();

    bool get(int devNum, sMapItem &item);
    int list(QList<int> &list);
    int size() {return mMap.size();}

protected:
    bool readFile();

private:
    devRtuMap mMap;
};

#endif // DEVRTUMAP_H
