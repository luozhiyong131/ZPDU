
INCLUDEPATH += $$PWD \
    $$PWD/data/hash/data \
    $$PWD/data/packet/base \
    $$PWD/data/packet \
    $$PWD/data/statistics \
    $$PWD/dev/devstore \
    $$PWD/dev/devList \
    

HEADERS += \
    $$PWD/data/packet/base/pdudatabase.h \
    $$PWD/data/packet/base/pdustrbase.h \
    $$PWD/data/packet/devdata/pdudataunit.h \
    $$PWD/data/packet/devdata/pdudevdata.h \
    $$PWD/data/packet/devdata/pduenvdata.h \
    $$PWD/data/packet/devdata/pduobjdata.h \
    $$PWD/data/packet/net/pdunetftp.h \
    $$PWD/data/packet/net/pdunethttp.h \
    $$PWD/data/packet/net/pdunetinfo.h \
    $$PWD/data/packet/net/pdunetip.h \
    $$PWD/data/packet/net/pdunetipaddr.h \
    $$PWD/data/packet/net/pdunetmodbus.h \
    $$PWD/data/packet/net/pdunetntp.h \
    $$PWD/data/packet/net/pdunetradius.h \
    $$PWD/data/packet/net/pdunetsmtp.h \
    $$PWD/data/packet/net/pdunetsnmp.h \
    $$PWD/data/packet/net/pdunetssh.h \
    $$PWD/data/packet/net/pdunettelnet.h \
    $$PWD/data/packet/net/pdunetwifi.h \
    $$PWD/data/packet/chart/pduchartinfo.h \
    $$PWD/data/packet/chart/pduchartth.h \
    $$PWD/data/packet/chart/pdudevchart.h \
    $$PWD/data/packet/devinfo/pdudevaddr.h \
    $$PWD/data/packet/devinfo/pdudevcpu.h \
    $$PWD/data/packet/devinfo/pdudevinfo.h \
    $$PWD/data/packet/devinfo/pdudevsys.h \
    $$PWD/data/packet/devinfo/pdudevtype.h \
    $$PWD/data/packet/output/pduoutput.h \
    $$PWD/data/packet/output/pduoutputname.h \
    $$PWD/data/packet/output/pduoutputsort.h \
    $$PWD/data/packet/devmanage/pducutoff.h \
    $$PWD/data/packet/devmanage/pdudevmanage.h \
    $$PWD/data/packet/devmanage/pdudevsetinfo.h \
    $$PWD/data/packet/devmanage/pdugroupsinfo.h \
    $$PWD/data/packet/devmanage/pduswcontrol.h \
    $$PWD/data/packet/devmanage/pdutimeswitch.h \
    $$PWD/data/packet/usr/pdudevusr.h \
    $$PWD/data/packet/usr/pdugrouprights.h \
    $$PWD/data/packet/usr/pduusrgroup.h \
    $$PWD/data/packet/usr/pduusrhash.h \
    $$PWD/data/packet/usr/pduusrmanager.h \
    $$PWD/data/packet/pdudatapacket.h \
    $$PWD/data/hash/data/pdudevhash.h \
    $$PWD/data/hash/data/pduhashdata.h \
    $$PWD/data/hash/data/pduhaship.h \
    $$PWD/data/hash/read/pduhashread.h \
    $$PWD/data/hash/read/pduhdckeck.h \
    $$PWD/data/hash/save/pduhashcom.h \
    $$PWD/data/hash/save/pduhashdatasave.h \
    $$PWD/data/hash/save/pduhashdevchartsave.h \
    $$PWD/data/hash/save/pduhashdevdatasave.h \
    $$PWD/data/hash/save/pduhashdevinfosave.h \
    $$PWD/data/hash/save/pduhashdevnetsave.h \
    $$PWD/data/hash/save/pduhashdevusrsave.h \
    $$PWD/data/hash/save/pduhashmanslave.h \
    $$PWD/data/hash/save/pduhashoutputsave.h \
    $$PWD/dev/devType/pdudtname.h \
    $$PWD/dev/devList/pdudevlist.h \
    $$PWD/dev/devList/pdudlobject.h \
    $$PWD/dev/offLine/pdu_offlinedev.h \
    $$PWD/dev/devAlarm/pdu_alarmdev.h \
    $$PWD/dev/devAlarm/pdu_alarmrecord.h \
    $$PWD/dev/devAlarm/pdu_clearflag.h \
    $$PWD/dev/pdudevspied.h \
    $$PWD/dev/devSetting/devsetting.h
    

	
SOURCES += \	
    $$PWD/data/packet/base/pdudatabase.cpp \
    $$PWD/data/packet/base/pdustrbase.cpp \
    $$PWD/data/packet/devdata/pdudataunit.cpp \
    $$PWD/data/packet/devdata/pdudevdata.cpp \
    $$PWD/data/packet/devdata/pduenvdata.cpp \
    $$PWD/data/packet/devdata/pduobjdata.cpp \
    $$PWD/data/packet/net/pdunetftp.cpp \
    $$PWD/data/packet/net/pdunethttp.cpp \
    $$PWD/data/packet/net/pdunetinfo.cpp \
    $$PWD/data/packet/net/pdunetip.cpp \
    $$PWD/data/packet/net/pdunetipaddr.cpp \
    $$PWD/data/packet/net/pdunetmodbus.cpp \
    $$PWD/data/packet/net/pdunetntp.cpp \
    $$PWD/data/packet/net/pdunetradius.cpp \
    $$PWD/data/packet/net/pdunetsmtp.cpp \
    $$PWD/data/packet/net/pdunetsnmp.cpp \
    $$PWD/data/packet/net/pdunetssh.cpp \
    $$PWD/data/packet/net/pdunettelnet.cpp \
    $$PWD/data/packet/net/pdunetwifi.cpp \
    $$PWD/data/packet/chart/pduchartinfo.cpp \
    $$PWD/data/packet/chart/pduchartth.cpp \
    $$PWD/data/packet/chart/pdudevchart.cpp \
    $$PWD/data/packet/devinfo/pdudevaddr.cpp \
    $$PWD/data/packet/devinfo/pdudevcpu.cpp \
    $$PWD/data/packet/devinfo/pdudevinfo.cpp \
    $$PWD/data/packet/devinfo/pdudevsys.cpp \
    $$PWD/data/packet/devinfo/pdudevtype.cpp \
    $$PWD/data/packet/output/pduoutput.cpp \
    $$PWD/data/packet/output/pduoutputname.cpp \
    $$PWD/data/packet/output/pduoutputsort.cpp \
    $$PWD/data/packet/devmanage/pducutoff.cpp \
    $$PWD/data/packet/devmanage/pdudevmanage.cpp \
    $$PWD/data/packet/devmanage/pdudevsetinfo.cpp \
    $$PWD/data/packet/devmanage/pdugroupsinfo.cpp \
    $$PWD/data/packet/devmanage/pduswcontrol.cpp \
    $$PWD/data/packet/devmanage/pdutimeswitch.cpp \
    $$PWD/data/packet/usr/pdudevusr.cpp \
    $$PWD/data/packet/usr/pdugrouprights.cpp \
    $$PWD/data/packet/usr/pduusrgroup.cpp \
    $$PWD/data/packet/usr/pduusrhash.cpp \
    $$PWD/data/packet/usr/pduusrmanager.cpp \
    $$PWD/data/packet/pdudatapacket.cpp \
    $$PWD/data/hash/data/pdudevhash.cpp \
    $$PWD/data/hash/data/pduhashdata.cpp \
    $$PWD/data/hash/data/pduhaship.cpp \
    $$PWD/data/hash/read/pduhashread.cpp \
    $$PWD/data/hash/read/pduhdckeck.cpp \
    $$PWD/data/hash/save/pduhashcom.cpp \
    $$PWD/data/hash/save/pduhashdatasave.cpp \
    $$PWD/data/hash/save/pduhashdevchartsave.cpp \
    $$PWD/data/hash/save/pduhashdevdatasave.cpp \
    $$PWD/data/hash/save/pduhashdevinfosave.cpp \
    $$PWD/data/hash/save/pduhashdevnetsave.cpp \
    $$PWD/data/hash/save/pduhashdevusrsave.cpp \
    $$PWD/data/hash/save/pduhashmanslave.cpp \
    $$PWD/data/hash/save/pduhashoutputsave.cpp \
    $$PWD/dev/devType/pdudtname.cpp \
    $$PWD/dev/devList/pdudevlist.cpp \
    $$PWD/dev/devList/pdudlobject.cpp \
    $$PWD/dev/offLine/pdu_offlinedev.cpp \
    $$PWD/dev/devAlarm/pdu_alarmdev.cpp \
    $$PWD/dev/devAlarm/pdu_alarmrecord.cpp \
    $$PWD/dev/devAlarm/pdu_clearflag.cpp \
    $$PWD/dev/pdudevspied.cpp \
    $$PWD/dev/devSetting/devsetting.cpp
    
	
   
	
