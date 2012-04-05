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

    serial_settings["baudRate"]    = BAUD9600;
    serial_settings["dataBits"]    = DATA_8;
    serial_settings["flowControl"] = FLOW_OFF;
    serial_settings["parity"]      = PAR_NONE;
    serial_settings["stopBits"]    = STOP_1;
    serial_settings["timeout"]     = 100;

#if 0
    {
        QList<TagMethod> tag_method_tablo;
        tag_method_tablo.append( TagMethod("tablo", "printText") );

        Porter::Pointer w = Porter::Pointer(new Porter(true));
        w->setScheduled(false);
        serial_settings["portName"] = "COM5";
        QMap<QString, QVariant> opts;
        opts["address"] = 0;

        w->setDevice("IoDeviceSerial", serial_settings);
        w->addDriver("DisplayCaptain", opts, tag_method_tablo);

        porters.push_back(w);
        //tags["tablo"]->appendFunc("value");
        //tags["tablo"]->setReadObject(w.data());
        tags["tablo"]->appendFunc("value", w.data() );
    }
#else
    {
        QList<TagMethod> tag_method_reader;
        tag_method_reader.append(TagMethod("reader1"));

        //tag_method_reader.append( TagMethod("reader1", "doOn") );
        //tag_method_reader.append( TagMethod("reader1", "doOff") );

        Porter::Pointer w = Porter::Pointer(new Porter(true));
        w->setScheduled(false);
        serial_settings["portName"] = "COM5";
        QMap<QString, QVariant> opts;
        opts["address"] = 2;

        w->setDevice("IoDeviceSerial", serial_settings);
        w->addDriver("MifareReader", opts, tag_method_reader);

        porters.push_back(w);


        tags["reader1"]->appendFunc("doOn", w.data(), "exec");
        tags["reader1"]->appendArgument("doOn", "doOn");

        tags["reader1"]->appendFunc("doOff", w.data(), "exec");
        tags["reader1"]->appendArgument("doOff", "doOff");

    }
#endif

    QList<TagMethod> tag_method_weight;
    tag_method_weight.append( TagMethod("weight1_1", "readWeight") );
    {
        Porter::Pointer w = Porter::Pointer(new Porter(true));

        serial_settings["portName"] = MrwSettings::instance()->platformaWeightPort[0];
        QMap<QString, QVariant> opts;
        opts["address"] = MrwSettings::instance()->platformaWeightAddress[0];
        opts["method"] = "readMethod";

        w->setDevice("IoDeviceSerial", serial_settings);
        w->addDriver(MrwSettings::instance()->platformaWeightType[0], opts, tag_method_weight);

        porters.push_back(w);
        tags["weight1_1"]->appendFunc("readMethod", w.data(), "value");

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
        tag_method_dido.append(TagMethod("dido", "getDi"));
        tag_method_dido.append(TagMethod("do", "setDo"));

        tag_method_dido.append(TagMethod("di1", "getDiBit"));
        tag_method_dido.append(TagMethod("di2", "getDiBit"));
        tag_method_dido.append(TagMethod("di3", "getDiBit"));
        tag_method_dido.append(TagMethod("di4", "getDiBit"));

        tag_method_dido.append(TagMethod("do1_tmp", "setDoBit"));
        tag_method_dido.append(TagMethod("do2_tmp", "setDoBit"));
        tag_method_dido.append(TagMethod("do3_tmp", "setDoBit"));
        tag_method_dido.append(TagMethod("do4_tmp", "setDoBit"));


        QMap<QString, QVariant> dido_port_settings;
        //dido_port_settings["deviceName"] = "\\\\.\\WDT_DEVICE";
        dido_port_settings["fileName"] = "dido.bin";
        //dido_port_settings["method"] = "readMethod";

        Porter::Pointer p = Porter::Pointer(new Porter(true));

        p->setScheduled(false);

        //p->setDevice("IoDeviceCtl", dido_port_settings);
        p->setDevice("IoDeviceFile", dido_port_settings);

        //p->addDriver("DidoIt8718f", QMap<QString, QVariant>(), tag_method_dido);
        p->addDriver("DidoFile", QMap<QString, QVariant>(), tag_method_dido);

        porters.push_back(p);



        tags["dido"]->appendFunc("readMethod" , p.data(), "value");

        tags["di1"]->appendFunc("readMethod", p.data(), "value");
        tags["di1"]->appendArgument("readMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["dido"], "readMethod")));
        tags["di1"]->appendArgument("readMethod", 0);

        tags["di2"]->appendFunc("readMethod", p.data(), "value");
        tags["di2"]->appendArgument("readMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["dido"], "readMethod")));
        tags["di2"]->appendArgument("readMethod", 1);

        tags["di3"]->appendFunc("readMethod", p.data(), "value");
        tags["di3"]->appendArgument("readMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["dido"], "readMethod")));
        tags["di3"]->appendArgument("readMethod", 2);

        tags["di4"]->appendFunc("readMethod", p.data(), "value");
        tags["di4"]->appendArgument("readMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["dido"], "readMethod")));
        tags["di4"]->appendArgument("readMethod", 3);

            tags["do1_tmp"]->appendFunc("writeMethod", p.data(), "value");
            tags["do1_tmp"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["dido"], "readMethod")));
            tags["do1_tmp"]->appendArgument("writeMethod", 0);
            tags["do1_tmp"]->appendArgument("writeMethod", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));

            tags["do2_tmp"]->appendFunc("writeMethod", p.data(), "value");
            tags["do2_tmp"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["dido"], "readMethod")));
            tags["do2_tmp"]->appendArgument("writeMethod", 1);
            tags["do2_tmp"]->appendArgument("writeMethod", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));

            tags["do3_tmp"]->appendFunc("writeMethod", p.data(), "value");
            tags["do3_tmp"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["dido"], "readMethod")));
            tags["do3_tmp"]->appendArgument("writeMethod", 2);
            tags["do3_tmp"]->appendArgument("writeMethod", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));

            tags["do4_tmp"]->appendFunc("writeMethod", p.data(), "value");
            tags["do4_tmp"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["dido"], "readMethod")));
            tags["do4_tmp"]->appendArgument("writeMethod", 3);
            tags["do4_tmp"]->appendArgument("writeMethod", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));
//qDebug () << "!!!!!!!!!!!!!!!!!!!1";
        tags["do1"]->appendFunc("writeMethod", p.data(), "value");
        tags["do1"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["do1_tmp"], "writeMethod")));
        //tags["do1"]->appendArgument("writeMethod", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(tags["do1_tmp"])));

        tags["do2"]->appendFunc("writeMethod", p.data(), "value");
        tags["do2"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["do2_tmp"], "writeMethod")));
        //tags["do2"]->appendArgument("writeMethod", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(tags["do2_tmp"])));

        tags["do3"]->appendFunc("writeMethod", p.data(), "value");
        tags["do3"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["do3_tmp"], "writeMethod")));
        //tags["do3"]->appendArgument("writeMethod", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(tags["do3_tmp"])));

        tags["do4"]->appendFunc("writeMethod", p.data(), "value");
        tags["do4"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["do4_tmp"], "writeMethod")));
        //tags["do4"]->appendArgument("writeMethod", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(tags["do4_tmp"])));

    }
}

void initTasks(TaskExec & tasks, Tags & tags, MainSequence & seq )
{
    PerimeterTask::Pointer perim (new PerimeterTask(tags));
    QMap<QString, QVariant> perim_settings;
/*    perim_settings["PerimeterType"] = "PerimeterControlByWeight";
    perim_settings["weightName"]    = "weight1_1";
    perim_settings["minWeight"]     = 50.0;
    perim_settings["method"]        = "readMethod"; */


    perim_settings["PerimeterType"] = "PerimeterControlByDi";
    perim_settings["AppearDi"]      = "di1";
    perim_settings["DisappearDi"]   = "di2";
    perim_settings["method"]        = "readMethod";

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

    tags["dido"]       = Tag::Pointer(new Tag("dido"));
    tags["di1"]        = Tag::Pointer(new Tag("di1"));
    tags["di2"]        = Tag::Pointer(new Tag("di2"));
    tags["di3"]        = Tag::Pointer(new Tag("di3"));
    tags["di4"]        = Tag::Pointer(new Tag("di4"));

    tags["do1"]        = Tag::Pointer(new Tag("do"));
    tags["do2"]        = Tag::Pointer(new Tag("do"));
    tags["do3"]        = Tag::Pointer(new Tag("do"));
    tags["do4"]        = Tag::Pointer(new Tag("do"));

    tags["do1_tmp"]        = Tag::Pointer(new Tag("do1_tmp"));
    tags["do2_tmp"]        = Tag::Pointer(new Tag("do2_tmp"));
    tags["do3_tmp"]        = Tag::Pointer(new Tag("do3_tmp"));
    tags["do4_tmp"]        = Tag::Pointer(new Tag("do4_tmp"));

    tags["reader1"]        = Tag::Pointer(new Tag("reader1"));

    QVector<Porter::Pointer> porters;


    initPorters(porters, tags);        

    MainSequence seq1(tags);

    TaskExec task_exec;

    initTasks(task_exec, tags, seq1);



    tags["di2"]->func("readMethod");
    tags["do1"]->func("writeMethod", Q_ARG(QVariant, true));
    //tags["di2"]->func("readMethod");

    //return 0;

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




