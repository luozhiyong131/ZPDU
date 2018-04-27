QT += core
QT -= gui
QT += sql
QT += network
QT += serialport
QT += websockets


CONFIG += c++11

TARGET = dips
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
DESTDIR = $$PWD/../bin

include(net/net.pri)
include(pdu/pdu.pri)
include(common/common.pri)
include(services/services.pri)
include(zebra/zebra.pri)
include(autoaddfriend/autoaddfriend.pri)
include(web/web.pri)

SOURCES += main.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS +=


DEFINES += ZEBRA_MODULE
#DEFINES += WEB_MODULE
