INCLUDEPATH += $$PWD

DEFINES += ZEBRA_MODULE
include(zebra_libs/zebra_libs.pri)

SOURCES += \
    $$PWD/zebra_peer.cpp \
    $$PWD/zebra_client.cpp \
    $$PWD/zebra_im.cpp \
    $$PWD/zebra_datapacket.cpp \
    $$PWD/zebra_jointhread.cpp \
    $$PWD/zebra_udpsocket.cpp
   

HEADERS  += \
    $$PWD/zebra_peer.h \
    $$PWD/zebra_client.h \
    $$PWD/zebra_im.h \
    $$PWD/zebra_datapacket.h \
    $$PWD/zebra_jointhread.h \
    $$PWD/zebra_udpsocket.h
    
