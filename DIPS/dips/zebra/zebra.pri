INCLUDEPATH += $$PWD/include

include(lib/lib.pri)

SOURCES += \
    $$PWD/zebra_delegates.cpp \
    $$PWD/zebra_client.cpp \
    $$PWD/zebra_objs.cpp \
    $$PWD/common_debug.cpp \
    $$PWD/signaldispatch.cpp \
    $$PWD/zebrdatasql.cpp

HEADERS  += \
    $$PWD/zebra_delegates.h \
    $$PWD/zebra_client.h \
    $$PWD/utils.h \
    $$PWD/common_debug.h \
    $$PWD/signaldispatch.h \
    $$PWD/zebrdatasql.h
