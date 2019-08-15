INCLUDEPATH += $$PWD

LIBS +=  -L$$PWD

LIBS += -lpeersafe_imapi
 LIBS += -lpeersafe_channel
 LIBS += -lpeersafe_core
 LIBS += -lpeersafe_dht
 LIBS += -lpeersafe_visit
 LIBS += -lpeersafe_peer
 LIBS += -ltorrent-rasterbar
 LIBS += -lprotobuf_lite
 LIBS += -lmaidsafe_common
 LIBS += -lmaidsafe_passport
 LIBS += -lcryptopp
 LIBS += -lboost_chrono-mt
 LIBS += -lboost_date_time-mt
 LIBS += -lboost_filesystem-mt
 LIBS += -lboost_regex-mt
 LIBS += -lboost_system-mt
 LIBS += -lboost_thread-mt
LIBS += -lboost_program_options-mt

SOURCES += \
   

HEADERS  += \
    $$PWD/im.h \
    $$PWD/event.h
    
