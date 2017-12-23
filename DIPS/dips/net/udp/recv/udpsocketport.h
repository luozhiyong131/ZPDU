#ifndef UDPSOCKETPORT_H
#define UDPSOCKETPORT_H

#include <QList>

void udp_port_add(int port);
int udp_port_size(void);
int udp_port_get(int i);
void udp_port_clear(void);

#endif // UDPSOCKETPORT_H
