#include <QtCore/QCoreApplication>
#include <QVector>

#include "func.h"
#include "mrwsettings.h"
#include "porter.h"
#include "qextserialport.h"
#include "tags.h"
#include "task.h"
#include "taskexec.h"
#include "perimeter.h"

bool initMrw();
int work();

void initPorters(QVector<Porter::Pointer>& porters, Tags& tags)
{
    QMap <QString, QVariant> serial_settings;

    serial_settings["baudRate"]    = BAUD9600;
    serial_settings["dataBits"]    = DATA_8;
    serial_settings["flowControl"] = FLOW_OFF;
    serial_settings["parity"]      = PAR_NONE;
    serial_settings["stopBits"]    = STOP_1;
    serial_settings["timeout"]     = 100;

    QList<TagMethod> tag_method_weight;
    tag_method_weight.append( TagMethod("weight1_1", "readWeight") );

    //tag_method_weight.append(TagMethod("weight1_2", "readWeight"));
    //tag_method_weight.append(TagMethod("weight1_3", "readWeight"));

    {
        Porter::Pointer w = Porter::Pointer(new Porter(true));

        serial_settings["portName"] = MrwSettings::instance()->platformaWeightPort[0];
        QMap<QString, QVariant> opts;
        opts["address"] = MrwSettings::instance()->platformaWeightAddress[0];

        w->setDevice("IoDeviceSerial", serial_settings);
        w->addDriver(MrwSettings::instance()->platformaWeightType[0], opts, tag_method_weight);
        //w->addDriver(MrwSettings::instance()->platformaWeightType[0], opts, tag_method_weight);
       // w->addDriver(MrwSettings::instance()->platformaWeightType[0], opts, tag_method_weight);

        porters.push_back(w);
        tags["weight1_1"]->setReadMethod("value");
        tags["weight1_1"]->setReadObject(w.data());

        //tags.insert("weight1_1", TagReadWrite::Pointer(new TagReadWrite(w.data(),)))

        //tags.insert("weight1_1", &Porter::value, w.data(), QString("weight1_1"));
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

        p->setDevice("IoDeviceCtl", dido_port_settings);

        p->addDriver("DidoIt8718f", QMap<QString, QVariant>(), tag_method_dido);

        //qDebug() << " di: "<<p->value("di", Q_ARG(int, 0));

        porters.push_back(p);

        //tags["di"]->setReadMethod("value");
        //tags["di"]->setReadObject(p.data());
    }
}

//#include <coroutine.h>

int main(int argc, char *argv[])
{  
    QCoreApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    printOnDisplay("Hello");



    MrwSettings::instance()->load("mrwsettings.xml");
//    MrwSettings::instance()->print();

    Tags tags;

    tags["weight1_1"] = Tag::Pointer(new Tag());
    tags["di"]        = Tag::Pointer(new Tag());
    tags["do"]        = Tag::Pointer(new Tag());       

    QVector<Porter::Pointer> porters;

    initPorters(porters, tags);
    //if (!initMrw()) return 1;

    TaskExec task_exec;

    task_exec.addTask(1000, Task::Pointer(new PerimeterTask(tags)));

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




