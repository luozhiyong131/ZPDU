#ifndef PDUSWCONTROL_H
#define PDUSWCONTROL_H
#include "pducutoff.h"

class PduSWControl : public PduCutOff
{
public:
    PduSWControl();

    void setSW(bool en);
    bool getSW(void);

private:
    bool sw; // 全开、全关状态
};

#endif // PDUSWCONTROL_H
