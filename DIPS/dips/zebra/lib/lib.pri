INCLUDEPATH += include

HEADERS  += $$PWD/../include/maidsafe/common/active.h \
        $$PWD/../include/peersafe/imapi/im.h \
        $$PWD/../include/peersafe/imapi/oid.h \


win32: {
    CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/x32/debug/peersafe/  -lpeersafe_imapi
        LIBS += -L$$PWD/x32/debug/peersafe/  -lpeersafe_channel
        LIBS += -L$$PWD/x32/debug/peersafe/  -lpeersafe_visit
        LIBS += -L$$PWD/x32/debug/peersafe/  -lpeersafe_nat
        LIBS += -L$$PWD/x32/debug/peersafe/  -lpeersafe_network
        LIBS += -L$$PWD/x32/debug/peersafe/  -ltorrent-rasterbar
        LIBS += -L$$PWD/x32/debug/peersafe/  -lpeersafe_oudp
        LIBS += -L$$PWD/x32/debug/peersafe/  -lprotobuf_lite
        LIBS += -L$$PWD/x32/debug/peersafe/  -lmaidsafe_common
        LIBS += -L$$PWD/x32/debug/peersafe/  -lmaidsafe_passport
        LIBS += -L$$PWD/x32/debug/peersafe/  -lcryptopp
        LIBS += -L$$PWD/x32/debug/boost/     -llibboost_chrono-vc120-mt-gd-1_57
        LIBS += -L$$PWD/x32/debug/boost/     -llibboost_date_time-vc120-mt-gd-1_57
        LIBS += -L$$PWD/x32/debug/boost/     -llibboost_filesystem-vc120-mt-gd-1_57
        LIBS += -L$$PWD/x32/debug/boost/     -llibboost_program_options-vc120-mt-gd-1_57
        LIBS += -L$$PWD/x32/debug/boost/     -llibboost_regex-vc120-mt-gd-1_57
        LIBS += -L$$PWD/x32/debug/boost/     -llibboost_system-vc120-mt-gd-1_57
        LIBS += -L$$PWD/x32/debug/boost/     -llibboost_thread-vc120-mt-gd-1_57
        LIBS += -L$$PWD/x32/debug/peersafe/  -ludt
    } else {
        LIBS += -L$$PWD/x32/release/peersafe/  -lpeersafe_imapi-msr
        LIBS += -L$$PWD/x32/release/peersafe/  -lpeersafe_channel-msr
        LIBS += -L$$PWD/x32/release/peersafe/  -lpeersafe_visit-msr
        LIBS += -L$$PWD/x32/release/peersafe/  -lpeersafe_nat-msr
        LIBS += -L$$PWD/x32/release/peersafe/  -lpeersafe_network-msr
        LIBS += -L$$PWD/x32/release/peersafe/  -ltorrent-rasterbar
        LIBS += -L$$PWD/x32/release/peersafe/  -lpeersafe_oudp-msr
        LIBS += -L$$PWD/x32/release/peersafe/  -lprotobuf_lite-msr
        LIBS += -L$$PWD/x32/release/peersafe/  -lmaidsafe_common-msr
        LIBS += -L$$PWD/x32/release/peersafe/  -lmaidsafe_passport-msr
        LIBS += -L$$PWD/x32/release/peersafe/  -lcryptopp-msr
        LIBS += -L$$PWD/x32/release/boost/     -llibboost_chrono-vc120-mt-1_57
        LIBS += -L$$PWD/x32/release/boost/     -llibboost_date_time-vc120-mt-1_57
        LIBS += -L$$PWD/x32/release/boost/     -llibboost_filesystem-vc120-mt-1_57
        LIBS += -L$$PWD/x32/release/boost/     -llibboost_program_options-vc120-mt-1_57
        LIBS += -L$$PWD/x32/release/boost/     -llibboost_regex-vc120-mt-1_57
        LIBS += -L$$PWD/x32/release/boost/     -llibboost_system-vc120-mt-1_57
        LIBS += -L$$PWD/x32/release/boost/     -llibboost_thread-vc120-mt-1_57
        LIBS += -L$$PWD/x32/release/peersafe/  -ludt-msr
    }
} else {
    unix:
    {
        CONFIG(debug, debug|release) {
            LIBS += -L$$PWD/linux32/debug/ -lcryptopp
            LIBS += -L$$PWD/linux32/debug/ -lpeersafe_imapi
            LIBS += -L$$PWD/linux32/debug/ -lmaidsafe_common
            LIBS += -L$$PWD/linux32/debug/ -lmaidsafe_passport
            LIBS += -L$$PWD/linux32/debug/ -lpeersafe_channel
            LIBS += -L$$PWD/linux32/debug/ -lpeersafe_nat
            LIBS += -L$$PWD/linux32/debug/ -lpeersafe_network
            LIBS += -L$$PWD/linux32/debug/ -lpeersafe_oudp
            LIBS += -L$$PWD/linux32/debug/ -lpeersafe_time
            LIBS += -L$$PWD/linux32/debug/ -lpeersafe_visit
            LIBS += -L$$PWD/linux32/debug/ -lprotobuf_lite
            LIBS += -L$$PWD/linux32/debug/ -ltorrent-rasterbar
            LIBS += -L$$PWD/linux32/debug/ -ludt

            LIBS += -L$$PWD/linux32/debug/ -lboost_chrono-mt
            LIBS += -L$$PWD/linux32/debug/ -lboost_date_time-mt
            LIBS += -L$$PWD/linux32/debug/ -lboost_filesystem-mt
            LIBS += -L$$PWD/linux32/debug/ -lboost_program_options-mt
            LIBS += -L$$PWD/linux32/debug/ -lboost_regex-mt
            LIBS += -L$$PWD/linux32/debug/ -lboost_system-mt
            LIBS += -L$$PWD/linux32/debug/ -lboost_thread-mt
        } else {
            LIBS += -L$$PWD/linux32/release/ -lcryptopp
            LIBS += -L$$PWD/linux32/release/ -lpeersafe_imapi
            LIBS += -L$$PWD/linux32/release/ -lmaidsafe_common
            LIBS += -L$$PWD/linux32/release/ -lmaidsafe_passport
            LIBS += -L$$PWD/linux32/release/ -lpeersafe_channel
            LIBS += -L$$PWD/linux32/release/ -lpeersafe_nat
            LIBS += -L$$PWD/linux32/release/ -lpeersafe_network
            LIBS += -L$$PWD/linux32/release/ -lpeersafe_oudp
            LIBS += -L$$PWD/linux32/release/ -lpeersafe_time
            LIBS += -L$$PWD/linux32/release/ -lpeersafe_visit
            LIBS += -L$$PWD/linux32/release/ -lprotobuf_lite
            LIBS += -L$$PWD/linux32/release/ -ltorrent-rasterbar
            LIBS += -L$$PWD/linux32/release/ -ludt

            LIBS += -L$$PWD/linux32/release/ -lboost_chrono-mt
            LIBS += -L$$PWD/linux32/release/ -lboost_date_time-mt
            LIBS += -L$$PWD/linux32/release/ -lboost_filesystem-mt
            LIBS += -L$$PWD/linux32/release/ -lboost_program_options-mt
            LIBS += -L$$PWD/linux32/release/ -lboost_regex-mt
            LIBS += -L$$PWD/linux32/release/ -lboost_system-mt
            LIBS += -L$$PWD/linux32/release/ -lboost_thread-mt
        }
    }
}

