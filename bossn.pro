#-------------------------------------------------
#
# Project created by QtCreator 2012-03-13T16:01:34
#
#-------------------------------------------------

#QMAKE_CXX = clang++
#
#INCLUDEPATH += d:/users/andigor/prg/mingw-x64-4.7.2-release-posix-sjlj-rev6.7/lib/gcc/x86_64-w64-mingw32/4.7.2/include/c++/ \
#               d:/users/andigor/prg/mingw-x64-4.7.2-release-posix-sjlj-rev6.7/lib/gcc/x86_64-w64-mingw32/4.7.2/include/c++/x86_64-w64-mingw32 \
#               d:/users/andigor/prg/mingw-x64-4.7.2-release-posix-sjlj-rev6.7/lib/gcc/x86_64-w64-mingw32/4.7.2/include/c++/tr1/

#INCLUDEPATH += d:/users/andigor/prg/STLport-5.2.1/stlport/

QT       += core xml sql script gui network
CONFIG   += uitools console

TARGET   = bossn
CONFIG   += console #precompile_header
CONFIG   -= app_bundle
DESTDIR  = build

TEMPLATE = app
INCLUDEPATH    += db alho serial weight generic iodevicewrapper scheduler           \
                  porter task dido perimeter tools reader  reports                  \
                  settings stable prot                                              \
                  alho/weighters alho/direction photograb                           \
                  alho/weighters/common coro gsoap


QMAKE_CXXFLAGS += -std=gnu++1y -Og #--param ggc-min-expand=10 --param ggc-min-heapsize=65535 -O

#QMAKE_CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer
#QMAKE_LFLAGS   += -fsanitize=address

#QMAKE_CXXFLAGS -= -pipe

#DEFINES += FUSION_MAX_VECTOR_SIZE=40 #DEBUG #SOAP_MEM_DEBUG

#\
#           BOOST_FUSION_INVOKE_MAX_ARITY=30                   \
#           BOOST_MPL_LIMIT_VECTOR_SIZE=30                    \
#           BOOST_RESULT_OF_NUM_ARGS=30                       \
#           BOOST_MULTI_INDEX_LIMIT_INDEXED_BY_SIZE=30        \
#           BOOST_MULTI_INDEX_LIMIT_TAG_SIZE=30               \
#           BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS=1

#PRECOMPILED_HEADER = qxorm_pch.h

LIBS           +=   -lqextserialport                \
                    -llibboost_context              \
                    #-lcoroutined                    \
                    #-lqxorm                         \
                    #-lboost_serialization           \
                    -llibmysql                      \
                    -lismm                          \
                    -llibboost_system               \
                    -llibboost_thread               \
                    -lboost_date_time               \
                    librdb \
                    -lwsock32

TRANSLATIONS = bossn_ua.ts

SOURCES +=  \
    alho/weighters/mria/beet/mriabeetacceptanceculture1.cpp \
    alho/weighters/mria/beet/mriabeetacceptanceculture2.cpp \
    alho/weighters/mria/beet/mriabeetacceptanceculture3.cpp \
    alho/weighters/mria/beet/mriabeetacceptanceculture4.cpp \
    alho/weighters/mria/beet/mriabeetacceptanceculture5.cpp \
\
    alho/weighters/kryzh/grain/kryzhgrainshipmentculture1.cpp \
    alho/weighters/kryzh/grain/kryzhgrainshipmentculture2.cpp \
    alho/weighters/kryzh/grain/kryzhgrainshipmentculture3.cpp \
    alho/weighters/kryzh/grain/kryzhgrainshipmentculture4.cpp \
    alho/weighters/kryzh/grain/kryzhgrainshipmentculture5.cpp \
\
    alho/weighters/kryzh/grain/kryzhgrainacceptanceculture1.cpp \
    alho/weighters/kryzh/grain/kryzhgrainacceptanceculture2.cpp \
    alho/weighters/kryzh/grain/kryzhgrainacceptanceculture3.cpp \
    alho/weighters/kryzh/grain/kryzhgrainacceptanceculture4.cpp \
    alho/weighters/kryzh/grain/kryzhgrainacceptanceculture5.cpp \
\
    alho/weighters/kryzh/beet/kryzhbeetacceptanceculture1.cpp   \
    alho/weighters/kryzh/beet/kryzhbeetacceptanceculture2.cpp   \
    alho/weighters/kryzh/beet/kryzhbeetacceptanceculture3.cpp   \
    alho/weighters/kryzh/beet/kryzhbeetacceptanceculture4.cpp   \
    alho/weighters/kryzh/beet/kryzhbeetacceptanceculture5.cpp   \
\
    alho/mainsequencebaseop.cpp \
    prot/prottask.cpp \
    alho/webservicesequence.cpp \
    main.cpp \
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
    iodevicewrapper/iodeviceconsole.cpp \
    iodevicewrapper/iodeviceemulator.cpp \   
    alho/weighters/kryzh/beet/kryzhbeetdbstructs.cpp \
    alho/weighters/kryzh/grain/kryzhgraindbstructs.cpp \    
    alho/weighters/kryzh/weighter/kryzhacceptanceweighter.cpp \
    alho/weighters/kryzh/weighter/kryzhshipmentweighter.cpp \
    alho/weighters/mria/beet/mriabeetdbstructs.cpp \
    alho/weighters/mria/weighter/mriaacceptanceweighter.cpp \
    reader/mifarereaderemulator.cpp \
    alho/reportsmanager2.cpp \
    coro/coroutine2.cpp \  
    gsoap/stdsoap2.cpp \
    gsoap/soapC.cpp \
    alho/mainsequencesettings.cpp \
    gsoap/soapbossnSoapBindingProxy.cpp \
    weight/SysTecIT3000.cpp \
    weight/VP05.cpp \
    display/displayposua.cpp \
    weight/swedaSW320.cpp \
    weight/elevatorpromservis.cpp \
    display/displaywinstar.cpp

HEADERS +=   reader/mifarereaderemulator.h \
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
    alho/weighters/constructweighters.h \
    perimeter/systemtrayiconreceiver_p.h \   
    iodevicewrapper/iodeviceconsole.h \
    iodevicewrapper/iodeviceemulator.h \
    alho/weighters/kryzh/beet/kryzhbeetdbstructs.h \
    alho/weighters/kryzh/beet/kryzhbeetacceptanceculture.h \
    alho/weighters/kryzh/grain/kryzhgrainacceptanceculture.h \
    alho/weighters/kryzh/grain/kryzhgraindbstructs.h \
    alho/weighters/kryzh/grain/kryzhgrainshipmentculture.h \
    alho/weighters/kryzh/weighter/kryzhacceptanceweighter.h \
    alho/weighters/kryzh/weighter/kryzhshipmentweighter.h \
    alho/weighters/mria/beet/mriabeetacceptanceculture.h \
    alho/weighters/mria/beet/mriabeetdbstructs.h \
    alho/weighters/mria/weighter/mriaacceptanceweighter.h \
    alho/weighters/kryzh/beet/kryzhbeettables.h \
    alho/bossnasync.h \
    alho/async_func2.h \
    tools/deleterlater.h \
    alho/weighters/common/generictables.h \
    rdb_pch.h \
    alho/reportsmanager2.h \
    coro/coroutine2.h \
    alho/weighters/kryzh/grain/kryzhgraintables.h \
    alho/weighters/mria/beet/mriabeettables.h \
    alho/webservicesequence.h \
    gsoap/stdsoap2.h \
    gsoap/soapH.h \
    alho/asyncfuncbase.h \
    alho/mainsequencebaseop.h \
    gsoap/stdsoap2.h \
    alho/tagprophelper.h \
    gsoap/soapbossnSoapBindingProxy.h \
    weight/SysTecIT3000.h \
    weight/VP05.h \
    display/displayposua.h \
    weight/swedaSW320.h \
    weight/elevatorpromservis.h \
    tools/fatal_exit.h \
    display/displaywinstar.h


RESOURCES += bossn.qrc


