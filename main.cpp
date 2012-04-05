#include <QtCore/QCoreApplication>
#include <QVector>

#include "func.h"
#include "mrwsettings.h"
#include "porter.h"
#include "qextserialport.h"
#include "tags.h"
#include "basetask.h"
#include "taskexec.h"
#include "perimeter.h"
#include "mainsequence.h"


bool initMrw();
int work();

void initPorters(QVector<Porter::Pointer>& porters, Tags& tags)
{
    QMap <QString, QVariant> serial_settings;

    serial_settings["baudRate"]    = BAUD19200;
    serial_settings["dataBits"]    = DATA_8;
    serial_settings["flowControl"] = FLOW_OFF;
    serial_settings["parity"]      = PAR_ODD;//PAR_NONE;
    serial_settings["stopBits"]    = STOP_1;
    serial_settings["timeout"]     = 100;

    {
        QList<TagMethod> tag_method_tablo;
        tag_method_tablo.append( TagMethod("tablo", "printText") );

        Porter::Pointer w = Porter::Pointer(new Porter(true));
        w->setScheduled(false);
        serial_settings["portName"] = "COM4";
        QMap<QString, QVariant> opts;
        opts["address"] = 0;
        w->setDevice("IoDeviceSerial", serial_settings);
       // w->addDriver("DisplayCaptain", opts, tag_method_tablo);
        w->addDriver("DisplayFutaba", opts, tag_method_tablo);

        porters.push_back(w);
        tags["tablo"]->setReadMethod("value");
        tags["tablo"]->setReadObject(w.data());
        //tags["tablo"]->addArgument("Hello");

    }

    QList<TagMethod> tag_method_weight;
    tag_method_weight.append( TagMethod("weight1_1", "readWeight") );
    {
        Porter::Pointer w = Porter::Pointer(new Porter(true));

        serial_settings["portName"] = MrwSettings::instance()->platformaWeightPort[0];
        QMap<QString, QVariant> opts;
        opts["address"] = MrwSettings::instance()->platformaWeightAddress[0];

        w->setDevice("IoDeviceSerial", serial_settings);
        w->addDriver(MrwSettings::instance()->platformaWeightType[0], opts, tag_method_weight);

        porters.push_back(w);
        tags["weight1_1"]->setReadMethod("value");
        tags["weight1_1"]->setReadObject(w.data());

        //tags.insert("weight1_1", TagReadWrite::Pointer(new TagReadWrite(w.data(),)))

        //tags.insert("weight1_1", &Porter::value, w.data(), QString("weight1_1"));
    }



/*    {
        Porter::Pointer w = Porter::Pointer(new Porter(true));

        serial_settings["portName"] = MrwSettings::instance()->platformaWeightPort[1];
        QMap<QString, QVariant> opts; opts["address"] = MrwSettings::instance()->platformaWeightAddress[1];

        w->setDevice("IoDeviceSerial", serial_settings);
        w->addDriver(MrwSettings::instance()->platformaWeightType[1], opts, tag_method_weight);

        porters.push_back(w);
    }*/


    {
        QList<TagMethod> tag_method_dido;
        tag_method_dido.append(TagMethod("di", "getDi"));
        //tag_method_dido.append(TagMethod("do", "setDo"));
        tag_method_dido.append(TagMethod("di1", "getDiBit"));
        tag_method_dido.append(TagMethod("di2", "getDiBit"));
        tag_method_dido.append(TagMethod("di3", "getDiBit"));
        tag_method_dido.append(TagMethod("di4", "getDiBit"));

        QMap<QString, QVariant> dido_port_settings;
        //dido_port_settings["deviceName"] = "\\\\.\\WDT_DEVICE";
        dido_port_settings["fileName"] = "dido.bin";

        Porter::Pointer p = Porter::Pointer(new Porter(true));

        p->setScheduled(false);

        //p->setDevice("IoDeviceCtl", dido_port_settings);
        p->setDevice("IoDeviceFile", dido_port_settings);

        //p->addDriver("DidoIt8718f", QMap<QString, QVariant>(), tag_method_dido);
        p->addDriver("DidoFile", QMap<QString, QVariant>(), tag_method_dido);

        porters.push_back(p);

        tags["di"]->setReadMethod("value");
        tags["di"]->setReadObject(p.data());

        tags["di1"]->setReadMethod("value");
        tags["di1"]->addArgument(QVariant::fromValue<Tag::WeakPointer>(tags["di"]));
        tags["di1"]->addArgument(0);
        tags["di1"]->setReadObject(p.data());

        tags["di2"]->setReadMethod("value");
        tags["di2"]->addArgument(QVariant::fromValue<Tag::WeakPointer>(tags["di"]));
        tags["di2"]->addArgument(1);
        tags["di2"]->setReadObject(p.data());

        tags["di3"]->setReadMethod("value");
        tags["di3"]->addArgument(QVariant::fromValue<Tag::WeakPointer>(tags["di"]));
        tags["di3"]->addArgument(2);
        tags["di3"]->setReadObject(p.data());

        tags["di4"]->setReadMethod("value");
        tags["di4"]->addArgument(QVariant::fromValue<Tag::WeakPointer>(tags["di"]));
        tags["di4"]->addArgument(3);
        tags["di4"]->setReadObject(p.data());

    }

}

void initTasks(TaskExec & tasks, Tags & tags, MainSequence & seq )
{
    PerimeterTask::Pointer perim (new PerimeterTask(tags));
    QMap<QString, QVariant> perim_settings;
/*    perim_settings["PerimeterType"] = "PerimeterControlByWeight";
    perim_settings["weightName"]    = "weight1_1";
    perim_settings["minWeight"]     = 50.0; */

    perim_settings["PerimeterType"] = "PerimeterControlByDi";
    perim_settings["AppearDi"]      = "di1";
    perim_settings["DisappearDi"]   = "di2";


    QObject::connect(perim.data(), SIGNAL(appeared()), &seq, SLOT(onAppearOnWeight()));
    QObject::connect(perim.data(), SIGNAL(disappeared()), &seq, SLOT(onDisappearOnWeight()));

    perim->setSettings(perim_settings);

    tasks.addTask(500, perim.staticCast<BaseTask::Pointer::Type>());
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    printOnDisplay("Hello");
    MrwSettings::instance()->load("mrwsettings.xml");
//    MrwSettings::instance()->print();

    Tags tags;

    tags["weight1_1"]  = Tag::Pointer(new Tag("weight1_1"));
    tags["tablo"]      = Tag::Pointer(new Tag("tablo"));

    tags["di"]         = Tag::Pointer(new Tag("di"));
    tags["di1"]        = Tag::Pointer(new Tag("di1"));
    tags["di2"]        = Tag::Pointer(new Tag("di2"));
    tags["di3"]        = Tag::Pointer(new Tag("di3"));
    tags["di4"]        = Tag::Pointer(new Tag("di4"));

    tags["do"]         = Tag::Pointer(new Tag("do"));
    tags["do1"]        = Tag::Pointer(new Tag("do1"));
    tags["do2"]        = Tag::Pointer(new Tag("do2"));
    tags["do3"]        = Tag::Pointer(new Tag("do3"));
    tags["do4"]        = Tag::Pointer(new Tag("do4"));

    QVector<Porter::Pointer> porters;

    initPorters(porters, tags);

    MainSequence seq1(tags);

    TaskExec task_exec;
    initTasks(task_exec, tags, seq1);


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




