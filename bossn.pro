#-------------------------------------------------
#
# Project created by QtCreator 2012-03-13T16:01:34
#
#-------------------------------------------------


QT       += core xml sql script gui
CONFIG   += uitools console

TARGET   = bossn
CONFIG   += console precompile_header
CONFIG   -= app_bundle
DESTDIR  = build

TEMPLATE = app
INCLUDEPATH    += ../qextserialport/src/ db alho serial weight generic iodevicewrapper scheduler   .          \
                    porter task dido perimeter tools reader  reports ../coroutine/src/ ../QxOrm/include       \
                                        $$(BOOST_ROOT) ../ismm/ settings stable prot  $$(MYSQL_DIR)/include   \
                                        QtSources/4.8.1/include alho/weighters alho/direction

QMAKE_CXXFLAGS += -std=c++0x #-m32
#QMAKE_LFLAGS   += -m32

PRECOMPILED_HEADER = qxorm_pch.h

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

TRANSLATIONS = bossn_ua.ts

SOURCES += main.cpp \
    weight/mettlertoledo8530.cpp \
    weight/mettlertoledo310.cpp \
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
    alho/weighters/graindbstructs.cpp \
	alho/weighters/beetdbstructs.cpp \
    reports/reports.cpp \
    iodevicewrapper/ismmdevice.cpp \
    alho/reportsmanager.cpp \
    settings/appsettings.cpp \
    settings/portersettings.cpp \
    settings/tasksettings.cpp \
    stable/stable.cpp \
    prot/prottask.cpp \
    prot/protdb.cpp \
    weight/hbm2108.cpp \
    weight/hbm2110.cpp \
    alho/mifarecarddata.cpp \
    alho/tagfunchelper.cpp \
    alho/seqdebug.cpp \
    alho/weighters/baseweighter.cpp \
    alho/initcardstructs.cpp \
    alho/weighters/baseacceptanceweighter.cpp \
    alho/weighters/beetacceptanceweighter.cpp \
    alho/weighters/grainacceptanceweighter.cpp \
    alho/weighters/grainshipmentweighter.cpp \
    alho/weighters/baseshipmentweighter.cpp \
    alho/weighters/weighterconf.cpp \
    alho/weighters/genericdbstructs.cpp \
    photograb/photomaker.cpp

HEADERS += \    
    tools/func.h \
    weight/mettlertoledo8530.h \
    weight/mettlertoledo310.h \
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
    reports/reports.h \    
    iodevicewrapper/ismmdevice.h \
    alho/async_func.h \
    alho/bossnexception.h \
    alho/reportsmanager.h \
    alho/warnmessages.h \
    alho/codeshacks.h \
    alho/conviencefuncs.h \
    settings/appsettings.h \
    settings/portersettings.h \
    settings/tasksettings.h \
    stable/stable.h \
    tools/tagmethod.h \
    alho/readersettings.h \
    prot/prottask.h \
    prot/protdb.h \
    weight/hbm2108.h \
    weight/hbm2110.h \
    alho/weighters/baseweighter.h \
    alho/mifarecarddata.h \
    alho/tagfunchelper.h \
    alho/mainsequencesettings.h \
    alho/seqdebug.h \
    alho/weighters/graindbstructs.h \
	alho/weighters/beetdbstructs.h \
    alho/weighters/baseacceptanceweighter.h \
    alho/weighters/baseshipmentweighter.h \
    alho/weighters/beetacceptanceweighter.h \
    alho/weighters/grainacceptanceweighter.h \
    alho/weighters/grainshipmentweighter.h \
    alho/weighters/weighterconf.h \
    alho/weighters/genericdbstructs.h \
    qxorm_pch.h \
    photograb/photomaker.h
	


