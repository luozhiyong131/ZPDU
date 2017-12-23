#ifndef UDPDATAQUE_H
#define UDPDATAQUE_H
#include "udprecvsocket.h"

void udp_data_add(UdpBaseData *data);
int udp_data_size(void);
UdpBaseData *udp_data_get(void);

#endif // UDPDATAQUE_H
