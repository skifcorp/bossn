#include <QtCore/QCoreApplication>
#include <QVector>

#include "func.h"
#include "tools.h"
#include "mrwsettings.h"
#include "porter.h"
#include "qextserialport.h"

bool initMrw();
int work();

void initPorters(QVector<Porter::Pointer>& porters)
{
    /*PortSettings s;
    s.BaudRate = BAUD9600;
    s.DataBits = DATA_8;
    s.FlowControl = FLOW_OFF;
    s.Parity = PAR_NONE;
    s.StopBits = STOP_1;
    s.Timeout_Millisec = 100;*/
    QMap <QString, QVariant> serial_settings;

    serial_settings["baudRate"]    = BAUD9600;
    serial_settings["dataBits"]    = DATA_8;
    serial_settings["flowControl"] = FLOW_OFF;
    serial_settings["parity"]      = PAR_NONE;
    serial_settings["stopBits"]    = STOP_1;
    serial_settings["timeout"]     = 100;

    QList<TagMethod> tag_method_weight;
    tag_method_weight.append(TagMethod("weight1_1", "readWeight"));
    tag_method_weight.append(TagMethod("weight1_2", "readWeight"));
    tag_method_weight.append(TagMethod("weight1_3", "readWeight"));

    {
        Porter::Pointer w = Porter::Pointer(new Porter(true));

        serial_settings["portName"] = MrwSettings::instance()->platformaWeightPort[0];
        QMap<QString, QVariant> opts; opts["address"] = MrwSettings::instance()->platformaWeightAddress[0];

        w->setDevice("IoDeviceSerial", serial_settings);
        w->addDriver(MrwSettings::instance()->platformaWeightType[0], opts, tag_method_weight);
        w->addDriver(MrwSettings::instance()->platformaWeightType[0], opts, tag_method_weight);
        w->addDriver(MrwSettings::instance()->platformaWeightType[0], opts, tag_method_weight);

        porters.push_back(w);
    }



    {
        Porter::Pointer w = Porter::Pointer(new Porter(true));

        serial_settings["portName"] = MrwSettings::instance()->platformaWeightPort[1];
        QMap<QString, QVariant> opts; opts["address"] = MrwSettings::instance()->platformaWeightAddress[1];

        w->setDevice("IoDeviceSerial", serial_settings);
        w->addDriver(MrwSettings::instance()->platformaWeightType[1], opts, tag_method_weight);

        porters.push_back(w);
    }

    {
        QList<TagMethod> tag_method_dido;
        tag_method_dido.append(TagMethod("di", "getDi"));
        tag_method_dido.append(TagMethod("do", "setDo"));

        QMap<QString, QVariant> dido_port_settings;
        dido_port_settings["deviceName"] = "\\\\.\\WDT_DEVICE";

        Porter::Pointer p = Porter::Pointer(new Porter(true));

        p->setScheduled(false);


        p->setDevice("IoDeviceDiscrete", dido_port_settings);
        p->addDriver("DidoDriver", QMap<QString, QVariant>(), tag_method_dido);

        qDebug() << " di: "<<p->value("di", Q_ARG(int, 0));

        porters.push_back(p);
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

    QVector<Porter::Pointer> porters;
    initPorters(porters);
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




