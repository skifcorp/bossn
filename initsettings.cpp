#include "initsettings.h"
#include "tags.h"
#include "porter.h"

#include <QList>
#include <QVariantMap>

#include "qextserialport.h"


void initTablo(QVector<Porter::Pointer>& porters, Tags & tags)
{
    QVariantMap serial_settings_tablo;

    serial_settings_tablo["baudRate"]    = BAUD19200;
    serial_settings_tablo["dataBits"]    = DATA_8;
    serial_settings_tablo["flowControl"] = FLOW_OFF;
    serial_settings_tablo["parity"]      = PAR_ODD;
    serial_settings_tablo["stopBits"]    = STOP_1;
    serial_settings_tablo["timeout"]     = 100;
    serial_settings_tablo["portName"]    = "COM4";

    QList<TagMethod> tag_method_tablo;
    tag_method_tablo.append( TagMethod("tablo") );

    Porter::Pointer w = Porter::Pointer(new Porter(true));
    w->setScheduled(false);

    w->setDevice("IoDeviceSerial", serial_settings_tablo);
    w->addDriver("DisplayFutaba", QVariantMap(), tag_method_tablo);

    porters.push_back(w);
    tags["tablo"]->appendFunc("print", w.data(), "exec" );
    tags["tablo"]->appendArgument("print", "printText");
    tags["tablo"]->appendArgument("print", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));

    qDebug() <<"tablo initialized!";
}

void initReader(QVector<Porter::Pointer>& porters, Tags & tags)
{
    QList<TagMethod> tag_method_reader;
    tag_method_reader.append(TagMethod("reader1"));


    Porter::Pointer w = Porter::Pointer(new Porter(true));
    w->setScheduled(false);

    QMap <QString, QVariant> serial_settings;

    serial_settings["baudRate"]    = BAUD9600;
    serial_settings["dataBits"]    = DATA_8;
    serial_settings["flowControl"] = FLOW_OFF;
    serial_settings["parity"]      = PAR_NONE;
    serial_settings["stopBits"]    = STOP_1;
    serial_settings["timeout"]     = 100;
    serial_settings["portName"]    = "COM3";

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

void initWeight(QVector<Porter::Pointer>& porters, Tags & tags)
{
    QMap <QString, QVariant> serial_settings;

    serial_settings["baudRate"]    = BAUD9600;
    serial_settings["dataBits"]    = DATA_8;
    serial_settings["flowControl"] = FLOW_OFF;
    serial_settings["parity"]      = PAR_NONE;
    serial_settings["stopBits"]    = STOP_1;
    serial_settings["timeout"]     = 100;
    serial_settings["portName"]    = "COM1" ;//MrwSettings::instance()->platformaWeightPort[0];

    QList<TagMethod> tag_method_weight;
    tag_method_weight.append( TagMethod("weight1_1", "readWeight") );

    Porter::Pointer w = Porter::Pointer(new Porter(true));

    QMap<QString, QVariant> opts;
    opts["address"] = 31;//MrwSettings::instance()->platformaWeightAddress[0];
    opts["method"] = "readMethod";

    w->setDevice("IoDeviceSerial", serial_settings);
    w->addDriver("Hbm2110", opts, tag_method_weight);

    porters.push_back(w);
    tags["weight1_1"]->appendFunc("readMethod", w.data(), "value");

}

void initDiDo(QVector<Porter::Pointer>& porters, Tags& tags)
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
//    dido_port_settings["fileName"] = "dido.bin";

    Porter::Pointer p = Porter::Pointer(new Porter(true));

    p->setScheduled(false);
    p->setDevice("IsmmDevice", dido_port_settings);
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
    opts.insert("platform_type"               , "auto" ); /*brutto or tara or auto*/
    opts.insert("database_driver"             , "QMYSQL");
    opts.insert("database_host"               , "192.168.0.69");
    opts.insert("database_name"               , "testdb");
    opts.insert("database_user"               , "root");
    opts.insert("database_password"           , "parabelum");
    opts.insert("brutto_delta_between_reweights", 10000);
    opts.insert("tara_delta_between_reweights"  , 10000);
    opts.insert("common_algorithm_of_analysis"  , "discrete" ); /*discrete or database_const*/
    opts.insert("common_size_of_group"          , 1);
    opts.insert("common_number_from_group"      , 0);
    opts.insert("corpotare_check_period_name"   , "ПериодичностьПроверкиМашинЮрЛиц");
    opts.insert("farmer_check_period_name"      , "ПериодичностьПроверкиМашинФермеров");
    opts.insert("bum11_name"                    , "ВесДляБум11");
    opts.insert("base_firm_name"                , "БазФирма");

    opts.insert("brutto_finish_pause"           , 10000 )  ;
    opts.insert("tara_finish_pause"             , 10000 )  ;
    opts.insert("finish_report_file_name"       , "finish_report.ui");
    opts.insert("start_report_file_name"        , "start_report.ui");
    opts.insert("tara_cars_mid_count_name"      , "КвоМашДляСредТары");
    opts.insert("tara_percent_name"             , "ПроцентДляТары");
    opts.insert("dont_check_time_name"          , "НеУчитыватьВремя");
    opts.insert("disp_phone_name"               , "ТелДиспетчера");

//  opts.insert("report_classes"                , QVariantList {"t_ttn" , "t_rez", "t_def_field", "t_bum",
//                                                                "t_cars", "t_kagat", "t_kontr", "t_field",
//                                                                "t_const", "t_bum_state_log"} );

}

void initTags(Tags & tags)
{
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
}
