INCLUDEPATH += $$PWD

#include($$PWD/lib/snmp.pri)

HEADERS += \
    $$PWD/autoaddfriend.h \
    $$PWD/auto_get_zebra_server_ip.h \
    $$PWD/zebrasecuritymanager.h

SOURCES += \
    $$PWD/autoaddfriend.cpp \
    $$PWD/auto_get_zebra_server_ip.cpp \
    $$PWD/zebrasecuritymanager.cpp
