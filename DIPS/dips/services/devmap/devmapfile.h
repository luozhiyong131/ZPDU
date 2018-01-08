#ifndef DEVMAPFILE_H
#define DEVMAPFILE_H
#include "common.h"

struct sMapItem {
    QString ip;
    int devNum;
};
typedef QMap<int, sMapItem> devRtuMap;

class DevMapFile
{
public:
    DevMapFile();

    bool readJson(devRtuMap &map);

protected:
    bool writeJson(const QString &name);
};

#endif // DEVMAPFILE_H
