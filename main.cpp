#include <QtCore/QCoreApplication>
#include <QVector>

#include "func.h"
#include "tools.h"
#include "mrwsettings.h"
#include "weighter.h"
#include "qextserialport.h"

bool initMrw();
int work();

void initWeighters(QVector<Weighter::Pointer>& weighters)
{
    /*PortSettings s;
    s.BaudRate = BAUD9600;
    s.DataBits = DATA_8;
    s.FlowControl = FLOW_OFF;
    s.Parity = PAR_NONE;
    s.StopBits = STOP_1;
    s.Timeout_Millisec = 100;*/
    QMap <QString, QVariant> settings;

    settings["baudRate"]    = BAUD9600;
    settings["dataBits"]    = DATA_8;
    settings["flowControl"] = FLOW_OFF;
    settings["parity"]      = PAR_NONE;
    settings["stopBits"]    = STOP_1;
    settings["timeout"]     = 100;

    {
        Weighter::Pointer w = Weighter::Pointer(new Weighter(true));

        settings["portName"] = MrwSettings::instance()->platformaWeightPort[0];
        QMap<QString, QVariant> opts; opts["address"] = MrwSettings::instance()->platformaWeightAddress[0];

        w->setWeightDevice("IoDeviceSerial", settings);
        w->addDriver(MrwSettings::instance()->platformaWeightType[0], opts);
        w->addDriver(MrwSettings::instance()->platformaWeightType[0], opts);
        w->addDriver(MrwSettings::instance()->platformaWeightType[0], opts);

        weighters.push_back(w);
    }
    return;
    {
        Weighter::Pointer w = Weighter::Pointer(new Weighter(true));

        settings["portName"] = MrwSettings::instance()->platformaWeightPort[1];
        QMap<QString, QVariant> opts; opts["address"] = MrwSettings::instance()->platformaWeightAddress[1];

        w->setWeightDevice("IoDeviceSerial", settings);
        w->addDriver(MrwSettings::instance()->platformaWeightType[1], opts);

        weighters.push_back(w);
    }
}

#include <coroutine.h>

int main(int argc, char *argv[])
{  
    QCoreApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    printOnDisplay("Hello");


    //QTimer timer;
    //timer.setInterval(500);
    //timer.setSingleShot(true);
    //QObject::connect(timer, SIGNAL(timeout()), []{qDebug() << "URAAAAA!";});

    MrwSettings::instance()->load("mrwsettings.xml");
//    MrwSettings::instance()->print();

    QVector<Weighter::Pointer> weighters;
    initWeighters(weighters);
    //if (!initMrw()) return 1;

    work();

    /*    if ( testArg == "-makedatabase_c") makeDataBaseC();
        if ( testArg == "-makedatabase_s") makeDataBaseS();
        if ( testArg == "-formatcard") formatCard(); */

    return app.exec();
}

bool initMrw()
{
    //ks.start();//поток для клавиатуры
    //mainWorker.start();//главный поток
    //dbW.start();//поток базы данных
    //DioHelper::instance()->start();//поток для дискретных входов


    //установим очередность потоков регистратора
    if (MrwSettings::instance()->platformaOff[0])
        printOnDisplay("Платформа №0 - отключена");
    else {
//        mw1.start();//поток считывателя и дисплея pl1
//        wWorker1.start();//поток весового контроллера
//        pW1.start();//принтер 1
//        action[0].start();
    }

    if (MrwSettings::instance()->platformaOff[1])
        printOnDisplay("Платформа №1 - отключена");
    else {
//        mw2.start();//поток считывателя и дисплея pl2
//        wWorker2.start();//поток весового контроллера
//        pW2.start();//принтер 2
//        action[1].start();
    }

//    toCardReaderTask[0].add(mrTaskFactory.getWriteDispText(0, "Загрузка ПО прошла успешно!"+endOfStr, 0, true));
//    toCardReaderTask[1].add(mrTaskFactory.getWriteDispText(1, "Загрузка ПО прошла успешно!"+endOfStr, 0, true));
    return true;
}


int work ()
{
    return 0;
}




