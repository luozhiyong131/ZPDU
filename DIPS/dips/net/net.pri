INCLUDEPATH +=  $$PWD/udp/recv \
                $$PWD/udp/sent \
                $$PWD/tcp/client \
                $$PWD/datadone \
                $$PWD/analyze \

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/udp/recv/udpsocketlist.h \
    $$PWD/udp/recv/udpsocketport.h \
    $$PWD/udp/recv/udpdataque.h \
    $$PWD/udp/sent/udpsentsocket.h \
    $$PWD/udp/sent/udpheartbeat.h \
    $$PWD/tcp/client/tcpclient.h \
    $$PWD/udp/recv/udprecvsocket.h \
    $$PWD/datadone/netanalyzedata.h \
    $$PWD/datadone/netpackdata.h \
    $$PWD/analyze/netdataanalyze.h \
    $$PWD/datadone/netdataformat.h \
    $$PWD/tcp/client/tcpsent.h \
    $$PWD/udp/sent/udpbdsent.h \
	
SOURCES += \
    $$PWD/udp/recv/udprecvsocket.cpp \
    $$PWD/udp/recv/udpsocketlist.cpp \
    $$PWD/udp/recv/udpsocketport.cpp \
    $$PWD/udp/recv/udpdataque.cpp \
    $$PWD/udp/sent/udpsentsocket.cpp \
    $$PWD/udp/sent/udpheartbeat.cpp \
    $$PWD/tcp/client/tcpclient.cpp \
    $$PWD/datadone/netanalyzedata.cpp \
    $$PWD/datadone/netpackdata.cpp \
    $$PWD/analyze/netdataanalyze.cpp \
    $$PWD/tcp/client/tcpsent.cpp \
    $$PWD/udp/sent/udpbdsent.cpp \
   
	
