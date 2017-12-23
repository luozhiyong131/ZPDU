#ifndef PDUOUTPUT_H
#define PDUOUTPUT_H
#include "pduoutputname.h"
#include "pduoutputsort.h"

class PduOutput
{
public:
    PduOutput();
    ~PduOutput();

    PduOutputName *name;
    PduOutputSort *sort;
};

#endif // PDUOUTPUT_H
