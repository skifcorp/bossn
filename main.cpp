//#include <QtCore/QCoreApplication>
#include <QCoreApplication>
#include <QVector>
#include <QSharedPointer>

#include "func.h"
#include "mrwsettings.h"
#include "porter.h"
#include "qextserialport.h"
#include "tags.h"
#include "basetask.h"
#include "taskexec.h"
#include "perimeter.h"
#include "mainsequence.h"
#include "formatersequence.h"
#include "alhosequence.h"


void initPorters(QVector<Porter::Pointer>& porters, Tags& tags)
{
    QMap <QString, QVariant> serial_settings;

    serial_settings["baudRate"]    = BAUD9600;
    serial_settings["dataBits"]    = DATA_8;
    serial_settings["flowControl"] = FLOW_OFF;
    serial_settings["parity"]      = PAR_NONE;
    serial_settings["stopBits"]    = STOP_1;
    serial_settings["timeout"]     = 100;


    {

        QMap <QString, QVariant> serial_settings_tablo;

        serial_settings_tablo["baudRate"]    = BAUD19200;
        serial_settings_tablo["dataBits"]    = DATA_8;
        serial_settings_tablo["flowControl"] = FLOW_OFF;
        serial_settings_tablo["parity"]      = PAR_ODD;
        serial_settings_tablo["stopBits"]    = STOP_1;
        serial_settings_tablo["timeout"]     = 100;
        serial_settings_tablo["portName"]    = "COM3";

        QList<TagMethod> tag_method_tablo;
        tag_method_tablo.append( TagMethod("tablo") );

        Porter::Pointer w = Porter::Pointer(new Porter(true));
        w->setScheduled(false);

        //QMap<QString, QVariant> opts;
        //opts["address"] = 0;

        w->setDevice("IoDeviceSerial", serial_settings_tablo);
        w->addDriver("DisplayFutaba", QVariantMap(), tag_method_tablo);

        porters.push_back(w);
        tags["tablo"]->appendFunc("print", w.data(), "exec" );
        tags["tablo"]->appendArgument("print", "printText");
        tags["tablo"]->appendArgument("print", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));

        //tags["tablo"]->setReadObject(w.data());
        //tags["tablo"]->appendFunc("value", w.data() );

        qDebug() <<"tablo initialized!";
    }



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

        tags["reader1"]->appendFunc("doSound", w.data(), "exec");
        tags["reader1"]->appendArgument("doSound", "doSound");
        tags["reader1"]->appendArgument("doSound", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));

        tags["reader1"]->appendFunc("activateIdleA", w.data(), "exec");
        tags["reader1"]->appendArgument("activateIdleA", "activateIdleA");

        tags["reader1"]->appendFunc("getHostCodedKey", w.data(), "exec");
        tags["reader1"]->appendArgument("getHostCodedKey", "getHostCodedKey");
        tags["reader1"]->appendArgument("getHostCodedKey", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));

        tags["reader1"]->appendFunc("doAuth", w.data(), "exec");
        tags["reader1"]->appendArgument("doAuth", "doAuth");
        tags["reader1"]->appendArgument("doAuth", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));

        tags["reader1"]->appendFunc("readBlock", w.data(), "exec");
        tags["reader1"]->appendArgument("readBlock", "readBlock");
        tags["reader1"]->appendArgument("readBlock", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));

        tags["reader1"]->appendFunc("writeBlock", w.data(), "exec");
        tags["reader1"]->appendArgument("writeBlock", "writeBlock");
        tags["reader1"]->appendArgument("writeBlock", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));
        tags["reader1"]->appendArgument("writeBlock", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(1)));

    }



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

        tags["do1"]->appendFunc("writeMethod", p.data(), "value");
        tags["do1"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["do1_tmp"], "writeMethod")));

        tags["do2"]->appendFunc("writeMethod", p.data(), "value");
        tags["do2"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["do2_tmp"], "writeMethod")));

        tags["do3"]->appendFunc("writeMethod", p.data(), "value");
        tags["do3"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["do3_tmp"], "writeMethod")));

        tags["do4"]->appendFunc("writeMethod", p.data(), "value");
        tags["do4"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["do4_tmp"], "writeMethod")));


    }

}

void initProgOptions(QVariantMap & opts)
{
    QByteArray card_code;
    card_code.append(0xFE);
    card_code.append(0x86);
    card_code.append(0x01);
    card_code.append(0x28);
    card_code.append(0x9F);
    card_code.append(0x3A);

    QByteArray default_card_code;
    default_card_code.append(0xFF);
    default_card_code.append(0xFF);
    default_card_code.append(0xFF);
    default_card_code.append(0xFF);
    default_card_code.append(0xFF);
    default_card_code.append(0xFF);

    opts.insert("card_code"                   , card_code);
    opts.insert("default_card_code"           , card_code);
    opts.insert("sleepnb_timeout"             , 1000);
    opts.insert("sleepnb_on_error_timeout"    , 10000);
    opts.insert("data_block"                  , 128/*144*/ );
    opts.insert("run_mode"                    , "prog" );
    opts.insert("format_with_default_data"    , true );
    opts.insert("platform_type"               , "brutto" ); /*or tara or auto*/
    opts.insert("database_driver"             , "QMYSQL");
    opts.insert("database_host"               , "127.0.0.1");
    opts.insert("database_name"               , "testdb");
    opts.insert("database_user"               , "root");
    opts.insert("database_password"           , "parabelum");
    opts.insert("brutto_delta_between_reweights", 10000);
    opts.insert("tara_delta_between_reweights"  , 10000);
    opts.insert("common_algorithm_of_analysis"  , "database_const" ); /*discrete or database_const*/
    opts.insert("common_size_of_group"          , 5);
    opts.insert("common_number_from_group"      , 10);
}

void initTasks(TaskExec & tasks, Tags & tags, MainSequence & seq )
{


    PerimeterTask::Pointer perim (new PerimeterTask(tags));
    QMap<QString, QVariant> perim_settings;
/*    perim_settings["PerimeterType"] = "PerimeterControlByWeight";
    perim_settings["weightName"]    = "weight1_1";
    perim_settings["minWeight"]     = 50.0;
    perim_settings["method"]        = "readMethod"; */

//    return ;

    perim_settings["PerimeterType"] = "PerimeterControlByDi";
    perim_settings["AppearDi"]      = "di1";
    perim_settings["DisappearDi"]   = "di2";
    perim_settings["method"]        = "readMethod";

//    return;

    //qDebug() << perim; return;

    QObject::connect(perim.data(), SIGNAL(appeared()), &seq, SLOT(onAppearOnWeight()), Qt::QueuedConnection );
    QObject::connect(perim.data(), SIGNAL(disappeared()), &seq, SLOT(onDisappearOnWeight()), Qt::QueuedConnection);



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

    tags["do1_tmp"]    = Tag::Pointer(new Tag("do1_tmp"));
    tags["do2_tmp"]    = Tag::Pointer(new Tag("do2_tmp"));
    tags["do3_tmp"]    = Tag::Pointer(new Tag("do3_tmp"));
    tags["do4_tmp"]    = Tag::Pointer(new Tag("do4_tmp"));

    tags["reader1"]    = Tag::Pointer(new Tag("reader1"));
    tags["database"]   = Tag::Pointer(new Tag("database"));

    QVector<Porter::Pointer> porters;


    initPorters(porters, tags);

    QMap<QString, QVariant> options;
    initProgOptions(options);

    TaskExec task_exec;    

    QSharedPointer<AlhoSequence> main_alho;

    if (options["run_mode"].toString() == "prog") {
        qDebug() << "run prog mode";

        main_alho = QSharedPointer<AlhoSequence>(new MainSequence(tags, options) );
        initTasks(task_exec, tags, *main_alho.staticCast<MainSequence>());
    }
    else if (options["run_mode"].toString() == "format") {
        qDebug() << "run format mode";

        main_alho = QSharedPointer<AlhoSequence>(new FormaterSequence(tags, options) );
        main_alho.staticCast<FormaterSequence>()->start();
    }


    //qDebug() << "hello";

    auto ret = app.exec();

    //qDebug () << "after exec";

    return ret;
}



