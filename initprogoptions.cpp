#include "initprogoptions.h"


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

    opts.insert("perimeter_control"           , true);
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
