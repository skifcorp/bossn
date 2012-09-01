#include "mainsequence.h"
#include "mifarecard.h"
#include "settingstool.h"
#include "func.h"

//#include "dbstructs.h"
#include "warnmessages.h"
#include "reportsmanager.h"
#include "codeshacks.h"
#include "conviencefuncs.h"

//#include "mysql.h"

#include "photograb/photomaker.h"

#include <QBitArray>





void MainSequence::setSettings(const QVariantMap & s)
{    
    alho_settings.green_light.tag_name = get_setting<QString>("green_light_tag", s);
    alho_settings.green_light.method_name = get_setting<QString>("green_light_method", s);

    alho_settings.red_light.tag_name = get_setting<QString>("red_light_tag", s);
    alho_settings.red_light.method_name = get_setting<QString>("red_light_method", s);

    alho_settings.tablo_tag.tag_name = get_setting<QString>("tablo_tag", s);
    alho_settings.tablo_tag.method_name = get_setting<QString>("tablo_method", s);

    alho_settings.perim_in.tag_name = get_setting<QString>("perim_in_tag", s);
    alho_settings.perim_in.method_name = get_setting<QString>("perim_in_method", s);

    alho_settings.perim_out.tag_name = get_setting<QString>("perim_out_tag", s);
    alho_settings.perim_out.method_name = get_setting<QString>("perim_out_method", s);

    alho_settings.weight_tag.tag_name = get_setting<QString>("weight_tag", s);
    alho_settings.weight_tag.method_name = get_setting<QString>("weight_method", s);

    alho_settings.weight_stable.tag_name = get_setting<QString>("stable_tag", s);
    alho_settings.weight_stable.method_name = get_setting<QString>("stable_method", s);

    alho_settings.logging.tag_name = get_setting<QString>("logging_tag", s);
    alho_settings.logging.method_name = get_setting<QString>("logging_method", s);

    alho_settings.reader.name                     = get_setting<QString>("reader_tag", s);
    alho_settings.reader.do_on.func_name          = get_setting<QString>("reader_do_on", s);
    alho_settings.reader.do_off.func_name         = get_setting<QString>("reader_do_off", s);
    alho_settings.reader.activate_idle.func_name  = get_setting<QString>("reader_activate_idle", s);
    alho_settings.reader.host_coded_key.func_name = get_setting<QString>("reader_host_coded_key", s);
    alho_settings.reader.do_auth.func_name        = get_setting<QString>("reader_do_auth", s);
    alho_settings.reader.write_block.func_name    = get_setting<QString>("reader_write_block", s);
    alho_settings.reader.read_block.func_name     = get_setting<QString>("reader_read_block", s);
    alho_settings.reader.do_sound.func_name       = get_setting<QString>("reader_sound", s);

    seq_id                              = get_setting<int>("id", s);

    //platform_type                       = get_setting<QString>("platform_type", s);
    printer_name                        = get_setting<QString>("printer_name", s);
    uses_photo                          = get_setting<bool>("uses_photo", s);

    if (uses_photo) {
        exit_photo                          = get_setting<QVariantMap>("exit_photo", s);
        enter_photo                         = get_setting<QVariantMap>("enter_photo", s);
    }

/*    qDebug() << "channel_num = " << exit_photo["channel_num"] << "\n";
    qDebug() <<"channel_alias = " << exit_photo["channel_alias"] << "\n";

    qDebug() << "channel_num = " << input_photo["channel_num"] << "n";
    qDebug() << "channel_alias = " << input_photo["channel_alias"] << "\n"; */


    setObjectName( "MainSequence num: " + QString::number(seq_id) );

    //weighters_conf = get_setting<QVariantList>("weighters_conf", s);

    initWeightersConf( s );

    restart();
    createStack(65535*8);
    cont();  //for initializing tablo and do

}

void MainSequence::initWeightersConf(const QVariantMap& s)
{
    QVariantList weighters_conf_list = get_setting<QVariantList>("weighters_conf", s );

    for (int i = 0; i<weighters_conf_list.count(); ++i) {
        bool ok = false;
        WeighterConf wc;

        auto m = weighters_conf_list[i].toMap();

        wc.database = QSqlDatabase::addDatabase(
              m.take("database_driver").toString(),
              m.take("connection_name").toString() + QString::number(seq_id));

        wc.material      = m.take("material").toInt(&ok);
        wc.weighter_name = m.take("weighter").toString();
        wc.platform_type = m.take("platform_type").toString();
        //wc.finish_report = weighters_conf_list[i].toMap()["finish_report"].toString();
        //wc.start_report  = weighters_conf_list[i].toMap()["start_report"].toString();

        wc.database.setHostName(m.take("database_host").toString());
        wc.database.setDatabaseName(m.take("database").toString());
        wc.database.setUserName(m.take("database_user").toString());
        wc.database.setPassword(m.take("database_password").toString());
        wc.database.setConnectOptions(m.take("connection_options").toString() );


        for ( QVariantMap::const_iterator iter =  m.constBegin(); iter != m.constEnd(); ++iter){
            //qDebug() << "k: " << iter.key() << " " << *iter;
            wc[iter.key()] = *iter;
        }

        if (!ok) {
            seqWarning() << "corruped material code... material index "<<i;
            continue;
        }

        //WeighterConf::initCardStruct(wc);

        weighters_conf.push_back(wc);
        qDebug() << m.take("photo_type").toString();
    }
}




void MainSequence::printOnTablo(const QString & s)
{
    tags[alho_settings.tablo_tag.tag_name]->func(alho_settings.tablo_tag.method_name, this, Q_ARG(const QVariant&, QVariant(s)));
}



MainSequence::MainSequence(Tags & t, const QVariantMap& s)
    :init(true),tags(t), app_settings(s), alho_settings(*this, tags) ,on_weight(false), seq_id(0), uses_photo(false), wake_timer(this)
{
    qx::QxSqlDatabase::getSingleton()->setTraceSqlQuery(false);
    qx::QxSqlDatabase::getSingleton()->setTraceSqlRecord(false);

    connect(&wake_timer, SIGNAL(timeout()), this, SLOT(wakeUp()));
    wake_timer.setSingleShot(true);
}


void MainSequence::checkForStealedCard(const ActivateCardISO14443A& prev_card, const ActivateCardISO14443A& card) const throw (MainSequenceException)
{
    if ( prev_card.uid.isEmpty() ) return;

    if ( prev_card.uid == card.uid ) return;

    throw MainSequenceException(tr(stealed_card_message), "You used stealed card!");
}


void MainSequence::wakeUp()
{
    cont();
}

void MainSequence::onAppearOnWeight(const QString& , AlhoSequence *)
{
    //qDebug ( ) << "status: " << status();

    if ( status() == NotStarted || status() == Terminated ) {
        restart();
        createStack(65535*8);
        wakeUp();
    }
}

WeighterConf& MainSequence::readStruct(MifareCardBlock & card, MifareCardData& d) throw (MifareCardException, MainSequenceException)
{
    QByteArray card_bytes = card.readByteArray( CardStructs::blocks_conf() );

    //seqWarning() << "card: " + byteArrayToString( card_bytes, 10 );

    int material = card_bytes[CardStructs::material_byte_address];
    WeighterConf& wc = findWeighterConf(material);

    d = card.readStruct(card_bytes, CardStructs::bill_conf(wc.weighter_name) );
    seqWarning(-1, false) << "card: " + card.toString(CardStructs::bill_conf(wc.weighter_name), d);

    qDebug() << card.toBigString(CardStructs::bill_conf(wc.weighter_name), d);

    return wc;
}


void MainSequence::makePhotoIfNeeded(long num_nakl, const QString& platform_type, const WeighterConf& wc)
{
    if (!uses_photo) return;

    QString str_exit  = get_setting<QString>("photo_dir", wc) + "\\" + QString::number(num_nakl) + "_"
            + platform_type + "_" + get_setting<QString>("channel_alias", exit_photo);
    QString str_input = get_setting<QString>("photo_dir", wc) + "\\" + QString::number(num_nakl) + "_"
            + platform_type + "_" + get_setting<QString>("channel_alias", enter_photo);

    capture.grabPhoto(str_exit.toStdWString().c_str()  , get_setting<QString>("channel_num", exit_photo ).toInt());
    capture.grabPhoto(str_input.toStdWString().c_str() , get_setting<QString>("channel_num", enter_photo).toInt());
}

void MainSequence::run()
{
    if ( init ) {
        printOnTablo(tr(greeting_message));
        setLightsToGreen();
        init = false;
        return;
    }



    seqDebug() << "something appeared on weight!!!! id" << seq_id;

    //capture.grabPhoto(L"D:\\SkifGate\\BazaDBF\\CH0_test_1", 0);

    on_weight = true;

    printOnTablo(tr(apply_card_message));

    setLightsToRed();


    //tags["reader1"]->func("doOn");
    //tags[ alho_settings.reader.name]->func( alho_settings.reader.do_on , this);
    alho_settings.reader.do_on.func();

    QByteArray card_code = get_setting<QByteArray>("card_code" , app_settings);
    uint data_block      = get_setting<uint>      ("data_block", app_settings);

    ActivateCardISO14443A cur_act;

    BaseWeighter::Pointer weighter;

    while(on_weight) {
        long cur_num_nakl = 0;

        //ActivateCardISO14443A act = tags[alho_settings.reader.name]->func(alho_settings.reader.activate_idle, this).value<ActivateCardISO14443A>();
        ActivateCardISO14443A act = alho_settings.reader.activate_idle.func().value<ActivateCardISO14443A>();
        MifareCardBlock card(act, alho_settings.reader, card_code, data_block);

        if ( !card.active() ) {
            //seqDebug() << "card not active!!!";
            sleepnbtm();
            //qDebug() << "after sleepnb";
            continue;
        }

        //seqDebug()<< "card active! platform: " + QString::number(seq_id);


        try {
            printOnTablo(tr(processing_message));

            //grabPhoto("_" + (weighter->detectPlatformType()) + "_" + exit_photo["channel_alias"], exit_photo["channel_num"]);


/*            if ( !database.isOpen() ) {
                if ( !async_func_ptr->async_call( [this]{return database.open();}) )
                    throw MainSequenceException(tr(error_database_lost), "Error database lost!!!");
            }*/

            card.autorize();

            MifareCardData bill;
            WeighterConf& weighter_conf = readStruct(card, bill);



            checkForStealedCard(cur_act, act); cur_act = act;




            processPerimeter();

            if (/*!bill.checkMember("billNumber", 0) ||*/ !bill.checkMember("driver", 0) ) {
                throw MainSequenceException(tr(card_is_empty_error_message), "bill is empty");
            }

            cur_num_nakl = bill.memberValue<int>("billNumber");

            if (!weighter)
                weighter = weighter_conf.createWeighter(*this);

            weighter->checkCardBanned( byteArrayToString(card.uid()) );
            weighter->processWeighting(bill, card, weighter_conf);

            makePhotoIfNeeded(cur_num_nakl, weighter->detectPlatformType(bill), weighter_conf );

            sleepnb( get_setting<int>("brutto_finish_pause", app_settings) );
            printOnTablo( tr(apply_card_message) );
            continue;

        }
        catch (MifareCardAuthException& ex) {
            seqWarning() << "auth_exeption! "<<ex.message() << " curNumNakl: " << cur_num_nakl;

            sleepnbtmerr(tr(card_autorize_error_message), tr(apply_card_message));
            continue;
        }
        catch (MifareCardReadException& ex) {
            seqWarning() << "read_card_exception! "<<ex.message() << " curNumNakl: "<< cur_num_nakl;
            sleepnbtmerr(ex.message(), tr(apply_card_message));
            continue;
        }
        catch (MifareCardWriteException& ex) {
            seqWarning() << "write_card_exception! "<<ex.message() << " curNumNakl: "<< cur_num_nakl;

            weighter->setCardBanned( byteArrayToString (card.uid()) );
            //seqWarning() << "after bannneddddd! ";

            sleepnbtmerr(ex.message(), tr(apply_card_message));
            continue;
        }
        catch (MainSequenceException& ex) {
            seqWarning()<<"sequence_exception: " << ex.adminMessage() << " curNumNakl: "<< cur_num_nakl << " sys: " + ex.systemMessage();
            sleepnbtmerr(ex.userMessage(), tr(apply_card_message));
            continue;
        }
    }

    seqDebug () << "\n\nexit from onAppearOnWeight!!!!!!!";

    printOnTablo(tr(greeting_message));
    setLightsToGreen();
    alho_settings.reader.do_off.func();
}




void MainSequence::onDisappearOnWeight(const QString&, AlhoSequence * )
{
    seqDebug() << "something disappeared on weight!!!!";

    on_weight = false;

    if ( wake_timer.isActive() ) {
        //qDebug( )     << "1";
        wake_timer.stop();
        //qDebug( )     << "2";

        if (  status() == Stopped  )
            wakeUp();
    }
    //qDebug( )     << "3";

    qDebug() << "disappear finished!!!";

    //qDebug( )     << "4";

    //tags[alho_settings.reader.name]->func( alho_settings.reader.do_off, this );

    //qDebug( )     << "5";

    //async_func_ptr->terminate();
    //qDebug( )     << "3";

    //qDebug( )     << "6";

//    printOnTablo(greeting_message);
//    setLightsToGreen();
}






void MainSequence::setLightsToRed()
{
    //tags["do1"]->func("writeMethod", Q_ARG(QVariant, 1));
    //tags["do2"]->func("writeMethod", Q_ARG(QVariant, 0));

    tags[alho_settings.red_light.tag_name]->func(alho_settings.red_light.method_name, this, Q_ARG(QVariant, 1));
    tags[alho_settings.green_light.tag_name]->func(alho_settings.green_light.method_name, this, Q_ARG(QVariant, 0));

}

void MainSequence::setLightsToGreen()
{
//    tags["do1"]->func("writeMethod", Q_ARG(QVariant, 0));
//    tags["do2"]->func("writeMethod", Q_ARG(QVariant, 1));
    tags[alho_settings.red_light.tag_name]->func(alho_settings.red_light.method_name, this, Q_ARG(QVariant, 0));
    tags[alho_settings.green_light.tag_name]->func(alho_settings.green_light.method_name, this, Q_ARG(QVariant, 1));
}




void MainSequence::processPerimeter() throw (MainSequenceException)
{
    if ( !get_setting<bool>("perimeter_control", app_settings) ) return;

//    if ( tags["di1"]->func("readMethod").toBool() || tags["di2"]->func("readMethod").toBool() )
//        throw MainSequenceException(perimeter_control_failed, "perimeter_control_failed!");

    if ( tags[ alho_settings.perim_in.tag_name ]->func(alho_settings.perim_in.method_name, this).toBool() ||
         tags[ alho_settings.perim_out.tag_name ]->func(alho_settings.perim_out.method_name, this).toBool() )
        throw MainSequenceException(tr(perimeter_control_failed), "perimeter_control_failed!");

}

/*const WeighterConf& MainSequence::findWeighterConf(const MifareCardData& bill)const throw (MainSequenceException)
{
    return findWeighterConf( bill.memberValue<int>( "material" ) );
}*/

WeighterConf& MainSequence::findWeighterConf(int material) throw (MainSequenceException)
{    
    for (int i = 0; i<weighters_conf.count(); ++i) {
        if ( weighters_conf[i].material == material )
            return weighters_conf[i];
    }

    throw MainSequenceException("cant find weighter", "cant find weighter for maaterial: " + material );
}

