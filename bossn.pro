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
INCLUDEPATH  += ../qextserialport/src/ serial weight generic iodevicewrapper scheduler porter  dido perimeter ../coroutine/src/
QMAKE_CXXFLAGS += -std=c++0x
LIBS         += -L../qextserialport_build/src/build/ -lqextserialport -L../coroutine_build/src/release/ -lcoroutine

SOURCES += main.cpp \
    mrwsettings.cpp \
    tools.cpp \
    weight/mettlertoledo8530.cpp \
    weight/mettlertoledo310.cpp \
    weight/hbm2110.cpp \
    weight/esitPWI.cpp \
    weight/weightfromfile.cpp \
    scheduler/scheduler.cpp \
    iodevicewrapper/iodevicewrapper.cpp \
    iodevicewrapper/iodeviceserial.cpp \
    iodevicewrapper/iodevicefile.cpp \ 
    porter/porter.cpp \
    dido/didodriver.cpp \
    iodevicewrapper/iodevicectl.cpp \
    perimeter/perimeter.cpp

HEADERS += \    
    func.h \
    mrwsettings.h \
    tools.h \
    weight/mettlertoledo8530.h \
    weight/mettlertoledo310.h \
    weight/hbm2110.h \
    weight/esitPWI.h \
    weight/weightfromfile.h \
    dido/didoer.h \
    dido/didodriver.h \
    scheduler/scheduler.h \
    iodevicewrapper/iodevicewrapper.h \
    iodevicewrapper/iodeviceserial.h \
    iodevicewrapper/iodevicefile.h \  
    porter/porter.h \   
    porter/porterdriver.h \
    iodevicewrapper/iodevicectl.h \
    perimeter/perimeter.h
