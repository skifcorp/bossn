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
INCLUDEPATH  += ../qextserialport/src/ serial weight generic iodevicewrapper scheduler porter task dido perimeter tools ../coroutine/src/
QMAKE_CXXFLAGS += -std=c++0x
LIBS         += -L../qextserialport_build/src/build/ -lqextserialport -L../coroutine_build/src/release/ -lcoroutine

SOURCES += main.cpp \
    mrwsettings.cpp \
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
    iodevicewrapper/iodevicectl.cpp \
    perimeter/perimeter.cpp \
    tags.cpp \
    perimeter/perimeteralhos.cpp \
    dido/didoIt8718f.cpp \
    dido/didofile.cpp

HEADERS += \    
    tools/func.h \
    mrwsettings.h \   
    weight/mettlertoledo8530.h \
    weight/mettlertoledo310.h \
    weight/hbm2110.h \
    weight/esitPWI.h \
    weight/weightfromfile.h \
    dido/didoer.h \
    scheduler/scheduler.h \
    iodevicewrapper/iodevicewrapper.h \
    iodevicewrapper/iodeviceserial.h \
    iodevicewrapper/iodevicefile.h \  
    porter/porter.h \   
    porter/porterdriver.h \
    iodevicewrapper/iodevicectl.h \
    perimeter/perimeter.h \
    tools/factory.h \
    tags.h \
    task/taskexec.h \
    task/basetask.h \
    perimeter/perimeteralhos.h \
    tools/settingstool.h \
    perimeter/perimtercontrol.h \
    mainsequence.h \
    dido/didoIt8718f.h \
    dido/didofile.h
