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
INCLUDEPATH    += db alho serial weight generic iodevicewrapper scheduler           \
                  porter task dido perimeter tools reader  reports                  \
                  settings stable prot                                              \
                  alho/weighters alho/direction photograb                           \
                  alho/weighters/common

QMAKE_CXXFLAGS += -std=c++0x #-m32
#QMAKE_LFLAGS   += -m32

PRECOMPILED_HEADER = qxorm_pch.h

CONFIG(debug, debug|release) {
LIBS           +=   -lqextserialportd               \
                    -lcoroutined                    \
                    -lqxormd                        \
                    -lboost_serialization           \
                    -llibmysql                      \
                    -lismm
#message("1111")
}

CONFIG(release, debug|release) {
LIBS           +=   -lqextserialport                \
                    -lcoroutine                     \
                    -lqxorm                         \
                    -lboost_serialization           \
                    -llibmysql                      \
                    -lismm
#message("2222")
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
    alho/initcardstructs.cpp \
    photograb/photomaker.cpp \
    reader/mifarereaderlist.cpp \
    weight/mettlertoledo8530_stream.cpp \
    alho/weighters/common/weighterconf.cpp \
    alho/weighters/common/weighter.cpp \
    alho/weighters/common/genericdbstructs.cpp \
    alho/weighters/common/culture.cpp \
    alho/weighters/kryzh/beet/beetdbstructs.cpp \
    alho/weighters/kryzh/beet/beetacceptanceculture.cpp \
    alho/weighters/kryzh/grain/grainshipmentculture.cpp \
    alho/weighters/kryzh/grain/graindbstructs.cpp \
    alho/weighters/kryzh/grain/grainacceptanceculture.cpp \
    alho/weighters/kryzh/weighter/shipmentweighter.cpp \
    alho/weighters/kryzh/weighter/acceptanceweighter.cpp


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
    alho/mifarecarddata.h \
    alho/tagfunchelper.h \
    alho/mainsequencesettings.h \
    alho/seqdebug.h \   	
    qxorm_pch.h \
    photograb/photomaker.h \ 
    reader/mifarereaderlist.h \
    weight/mettlertoledo8530_stream.h \
    alho/weighters/common/weighterconf.h \
    alho/weighters/common/weighter.h \
    alho/weighters/common/shipmentculture.h \
    alho/weighters/common/genericdbstructs.h \
    alho/weighters/common/culture.h \
    alho/weighters/common/acceptanceculture.h \
    alho/weighters/kryzh/beet/beetdbstructs.h \
    alho/weighters/kryzh/beet/beetacceptanceculture.h \
    alho/weighters/kryzh/grain/grainshipmentculture.h \
    alho/weighters/kryzh/grain/graindbstructs.h \
    alho/weighters/kryzh/grain/grainacceptanceculture.h \
    alho/weighters/kryzh/weighter/shipmentweighter.h \
    alho/weighters/kryzh/weighter/acceptanceweighter.h \
    alho/weighters/constructweighters.h
	
