#ifndef PDUCHARTINFO_H
#define PDUCHARTINFO_H
#include "pdudatabase.h"
#include "pdustrbase.h"

class PduChartInfo
{
public:
    PduChartInfo();
    ~PduChartInfo();

    PduDataBase *data;
    PduStrBase *time; // 预留
};

#endif // PDUCHARTINFO_H
