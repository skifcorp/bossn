#-------------------------------------------------
#
# Project created by QtCreator 2012-03-13T16:01:34
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = bossn
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
INCLUDEPATH  += ../qextserialport/src/ serial weight generic ../coroutine/src/
QMAKE_CXXFLAGS += -std=gnu++0x
LIBS         += -L../qextserialport_build/src/build/ -lqextserialport -L../coroutine_build/src/release/ -lcoroutine

SOURCES += main.cpp \
    mrwsettings.cpp \
    tools.cpp \
    weight/weighter.cpp \
    weight/mettlertoledo8530.cpp \
    weight/mettlertoledo310.cpp \
    weight/hbm2110.cpp \
    weight/esitPWI.cpp \
    weight/weightfromfile.cpp \
    weight/weightdevice.cpp \
    weight/weightdeviceserial.cpp \
    weight/weightdevicefile.cpp

HEADERS += \    
    func.h \
    mrwsettings.h \
    tools.h \
    weight/weighter.h \
    weight/mettlertoledo8530.h \
    weight/mettlertoledo310.h \
    weight/hbm2110.h \
    weight/esitPWI.h \
    weight/weightfromfile.h \
    weight/weight.h \
    weight/weightdevice.h \
    weight/weightdeviceserial.h \
    weight/weightdevicefile.h
