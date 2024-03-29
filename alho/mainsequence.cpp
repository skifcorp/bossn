#include <QBitArray>

#include "rdb_pch.h"

#include "mainsequence.h"
#include "mifarecard.h"
#include "settingstool.h"
#include "func.h"


#include "warnmessages.h"
#include "reportsmanager.h"
#include "codeshacks.h"
#include "conviencefuncs.h"

#include "photomaker.h"

#include "weighterconf.h"

#include <memory>

/*
alho::common::Weighter::Pointer MainSequence::createWeighter (WeighterConf& wc)
{
    return alho::common::Weighter::create( wc.weighter_name, *this, wc.database, *wc.db );
}
*/

MainSequence::~MainSequence()
{

}

void MainSequence::setSettings(const QVariantMap & s)
{    
    alho_settings.init(s);

    seq_id                              = get_setting<int>("id", s);
    printer_name                        = get_setting<QString>("printer_name", s, QString());
    uses_photo                          = get_setting<bool>("uses_photo", s, false);

    if (uses_photo) {
        exit_photo                          = get_setting<QVariantMap>("exit_photo", s);
        enter_photo                         = get_setting<QVariantMap>("enter_photo", s);
    }

    //current_card_tag = get_setting<QString>("current_card_tag", s);
    //current_card_prop = get_setting<QString>("current_card_prop", s);

    setObjectName( "MainSequence num: " + QString::number(seq_id) );

    initWeightersConf( s );

    restart();

    //createStack(65535*16);
    cont();  //for initializing tablo and do
}

void MainSequence::initWeightersConf(const QVariantMap& s)
{
    QVariantList weighters_conf_list = get_setting<QVariantList>("weighters_conf", s );

    for (int i = 0; i<weighters_conf_list.count(); ++i) {
        bool ok = false;
        std::unique_ptr<WeighterConf> wc(new WeighterConf);

        auto m = weighters_conf_list[i].toMap();

        wc->database = QSqlDatabase::addDatabase(
              m.take("database_driver").toString(),
              m.take("connection_name").toString() + QString::number(seq_id));

        wc->material      = m.take("material").toInt(&ok);
        wc->weighter_name = m.take("weighter").toString();
        wc->platform_type = m.take("platform_type").toString();

        wc->database.setHostName(m.take("database_host").toString());
        wc->database.setDatabaseName(m.take("database").toString());
        wc->database.setUserName(m.take("database_user").toString());
        wc->database.setPassword(m.take("database_password").toString());
        wc->database.setConnectOptions(m.take("connection_options").toString() );


        wc->db->setHost(wc->database.hostName().toStdString());
        wc->db->setDatabase(wc->database.databaseName().toStdString());
        wc->db->setUser(wc->database.userName().toStdString());
        wc->db->setPassword(wc->database.password().toStdString());

        for ( QVariantMap::const_iterator iter =  m.constBegin(); iter != m.constEnd(); ++iter){
            //qDebug() << "k: " << iter.key() << " " << *iter;
            (*wc)[iter.key()] = *iter;
        }

        if (!ok) {
            seqWarning() << "corruped material code... material index "<<i;
            continue;
        }

        //WeighterConf::initCardStruct(wc);

        weighters_conf.push_back( std::move(wc) );

    }
}





MainSequence::MainSequence(Tags & t, const QVariantMap& s)
    :MainSequenceBaseOp("MainSequence", t, s)
{
#if 0
    qx::QxSqlDatabase::getSingleton()->setTraceSqlQuery(false);
    qx::QxSqlDatabase::getSingleton()->setTraceSqlRecord(false);
#endif

}


/*
void MainSequence::checkForStealedCard(const ActivateCardISO14443A& card)
{
    if ( !tags[current_card_tag]->containsProp(current_card_prop) ||
         !tags[current_card_tag]->getProperty<ActivateCardISO14443A>(current_card_prop).active() ) {
        tags[current_card_tag]->setProperty(current_card_prop,
              QVariant::fromValue<ActivateCardISO14443A>(card));
        return;
    }

    if ( tags[current_card_tag]->getProperty<ActivateCardISO14443A>(current_card_prop).uid ==
         card.uid) return;

    throw MainSequenceException(tr(stealed_card_message), "You used stealed card!");
}
*/

void MainSequence::wakeUp()
{   
    cont();
}

void MainSequence::onAppearOnWeight(const QString& , AlhoSequence *)
{
    //qDebug ( ) << "status: " << status();

    if ( status() == NotStarted || status() == Terminated ) {
        restart();
        //createStack(65535*16);
        wakeUp();
    }
}

WeighterConf& MainSequence::readStruct(MifareCardSector & card, MifareCardData& d)
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

    on_weight = true;

    printOnTablo(tr(apply_card_message));

    setLightsToRed();

    alho_settings.reader.all_readers_do_on();

    QByteArray card_code = get_setting<QByteArray>("card_code" , app_settings);
    uint data_block      = get_setting<uint>      ("data_block", app_settings);

    //ActivateCardISO14443A cur_act;

    alho::common::Weighter::Pointer weighter;

    std::shared_ptr<MainSequence> current_card_guard( this, [this](MainSequence *){
        alho_settings.current_card.setProperty(QVariant::fromValue<ActivateCardISO14443A>(ActivateCardISO14443A()));
    }); Q_UNUSED(current_card_guard);

    while(on_weight) {

        long cur_num_nakl = 0;

        //ActivateCardISO14443A act = tags[alho_settings.reader.name]->func(alho_settings.reader.activate_idle, this).value<ActivateCardISO14443A>();
        std::pair<ActivateCardISO14443A, int> act = alho_settings.reader.all_readers_activate_idle();
        MifareCardSector card(act.first, alho_settings.reader[act.second], card_code, data_block);

        if ( !card.active() ) {
            //seqDebug() << "card not active!!!";
            sleepnbtm();
            //qDebug() << "after sleepnb";
            continue;
        }

        //seqDebug()<< "card active! platform: " + QString::number(seq_id);


        try {
            printOnTablo(tr(processing_message));

/*            if ( !database.isOpen() ) {
                if ( !async_func_ptr->async_call( [this]{return database.open();}) )
                    throw MainSequenceException(tr(error_database_lost), "Error database lost!!!");
            }*/

            card.autorize();

            MifareCardData bill;
            WeighterConf& weighter_conf = readStruct(card, bill);

            checkForStealedCard( act.first );

            //processPerimeter();

            if (/*!bill.checkMember("billNumber", 0) ||*/ !bill.checkMember("driver", 0) ) {
                throw MainSequenceException(tr(card_is_empty_error_message), "bill is empty");
            }

            cur_num_nakl = bill.memberValue<int>("billNumber");

            if (!weighter)
                //weighter = createWeighter(weighter_conf);

                weighter = alho::common::Weighter::create(
                            weighter_conf.weighter_name,
                            *this,
                            weighter_conf.database,
                            *weighter_conf.db );

            weighter->checkCardBanned( byteArrayToString(card.uid()) );
            weighter->processWeighting(bill, card, weighter_conf);

            //makePhotoIfNeeded(cur_num_nakl, weighter->detectPlatformType(bill), weighter_conf );

            sleepnb( get_setting<int>("brutto_finish_pause", app_settings) );
            printOnTablo( tr(apply_card_message) );
            continue;

        }
        catch (MifareCardAuthException& ex) {
            seqWarning() << "auth_exeption! "<<ex.adminMessage() << " curNumNakl: " << cur_num_nakl;

            sleepnbtmerr(tr(card_autorize_error_message), tr(apply_card_message));
            continue;
        }
        catch (MifareCardReadException& ex) {
            seqWarning() << "read_card_exception! "<<ex.adminMessage() << " curNumNakl: "<< cur_num_nakl;
            sleepnbtmerr(ex.userMessage(), tr(apply_card_message));
            continue;
        }
        catch (MifareCardWriteException& ex) {
            seqWarning() << "write_card_exception! "<<ex.adminMessage() << " curNumNakl: "<< cur_num_nakl;

            weighter->setCardBanned( byteArrayToString (card.uid()) );
            //seqWarning() << "after bannneddddd! ";

            sleepnbtmerr(ex.userMessage(), tr(apply_card_message));
            continue;
        }
        catch (MainSequenceException& ex) {
            seqWarning()<<"sequence_exception: " << ex.adminMessage() << " curNumNakl: "<< cur_num_nakl << " sys: " + ex.systemMessage();
            sleepnbtmerr(ex.userMessage(), tr(apply_card_message));
            continue;
        }
        catch (MifareCardException& ex) {
            seqWarning()<<"mifare_card_exception: " << ex.adminMessage();
            sleepnbtmerr(ex.userMessage(), tr(apply_card_message));
            continue;
        }
    }

    seqDebug () << "\n\nexit from onAppearOnWeight!!!!!!!";

    printOnTablo(tr(greeting_message));
    setLightsToGreen();
    //tags[current_card_tag]->setProperty(current_card_prop,
    //                                   QVariant::fromValue<ActivateCardISO14443A>(ActivateCardISO14443A()));
    //alho_settings.current_card.setProperty(QVariant::fromValue<ActivateCardISO14443A>(ActivateCardISO14443A()));
    alho_settings.reader.all_readers_do_off();
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
}


void MainSequence::processPerimeter()
{
    if ( !get_setting<bool>("perimeter_control", app_settings) ) return;

    if ( alho_settings.perim_in.func().toBool() == alho_settings.perim_in.value.toBool() ||
         alho_settings.perim_out.func().toBool() == alho_settings.perim_out.value.toBool() )
        throw MainSequenceException(tr(perimeter_control_failed), "perimeter_control_failed!");
}

WeighterConf& MainSequence::findWeighterConf(int material)
{    
    for (uint i = 0; i<weighters_conf.size(); ++i) {
        if ( weighters_conf[i]->material == material )
            return *weighters_conf[i];
    }

    throw MainSequenceException("cant find weighter", "cant find weighter for maaterial: " + material );
}

