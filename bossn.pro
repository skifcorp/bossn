  #-------------------------------------------------
#
# Project created by QtCreator 2012-03-13T16:01:34
#
#-------------------------------------------------


QT       += core xml sql script gui
CONFIG   += uitools

TARGET   = bossn
CONFIG   += console
CONFIG   -= app_bundle
DESTDIR  = build

TEMPLATE = app
INCLUDEPATH    += ../qextserialport/src/ db alho serial weight generic iodevicewrapper scheduler           \
                      porter task dido perimeter tools reader  reports ../coroutine/src/ ../QxOrm/include  \
                      $$(BOOST_ROOT) ../ismm/
QMAKE_CXXFLAGS += -std=c++0x #-m32
#QMAKE_LFLAGS   += -m32

CONFIG(debug, debug|release) {
LIBS           +=   -L../qextserialport_build/src/build/ -lqextserialportd               \
                    -L../coroutine_build/src/build/ -lcoroutined                         \
                    -L../QxOrm_build/build/ -lqxormd                                     \
                    -L $$(BOOST_ROOT)/stage/lib -llibboost_serialization-mgw46-mt-d-1_48 \
                    -L $$(MYSQL_DIR)/lib -llibmysql                                      \
                    -L ../ismm/ -lismm
} else {
LIBS           +=   -L../qextserialport_build/src/build/ -lqextserialport               \
                    -L../coroutine_build/src/build/ -lcoroutine                         \
                    -L../QxOrm_build/build/ -lqxorm                                     \
                    -L $$(BOOST_ROOT)/stage/lib -llibboost_serialization-mgw46-mt-1_48  \
                    -L $$(MYSQL_DIR)/lib -llibmysql                                     \
                    -L ../ismm/ -lismm
}

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
    dido/didofile.cpp \
    display/displaycaptain.cpp \
    reader/mifarereader.cpp \
    display/displayfutaba.cpp \
    dido/didow83627.cpp \
    alho/mainsequence.cpp \
    alho/mifarecard.cpp \
    alho/formatersequence.cpp \
    alho/alhosequence.cpp \
    alho/cardstructs.cpp \
    alho/dbstructs.cpp \
    reports/reports.cpp \
    initsettings.cpp \
    iodevicewrapper/ismmdevice.cpp \
    alho/reportsmanager.cpp \
    initprogoptions.cpp

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
    dido/didofile.h \
    display/displaycaptain.h \
    reader/mifarereader.h \
    tools/crctool.h \
    display/displayfutaba.h \
    dido/didow83627.h \
    tools/sharedfromthis.h \
    alho/mifarecard.h \
    alho/mainsequence.h \
    alho/formatersequence.h \
    alho/alhosequence.h \
    alho/cardstructs.h \
    alho/dbstructs.h \
    reports/reports.h \
    initsettings.h \
    iodevicewrapper/ismmdevice.h \
    alho/async_func.h \
    alho/bossnexception.h \
    alho/reportsmanager.h \
    alho/warnmessages.h \
    alho/codeshacks.h \
    alho/conviencefuncs.h \
    initprogoptions.h
