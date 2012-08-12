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

#if 0
    database = QSqlDatabase::addDatabase(get_setting<QString>("database_driver", s), "sequ_" + QString::number(seq_id));

    database.setHostName(get_setting<QString>("database_host", s));
    database.setDatabaseName(get_setting<QString>("database_name", s));
    database.setUserName(get_setting<QString>("database_user", s));
    database.setPassword(get_setting<QString>("database_password", s));
    database.setConnectOptions( get_setting<QString>("connection_options", s, QString() ));

    async_func_ptr     = async_func::pointer( new  async_func(database, *this) );
    convience_func_ptr = convience_func::pointer( new convience_func (*async_func_ptr) );
#endif
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
    }
}

#if 0
QString MainSequence::taraFinishMessage(int tara) const
{
    return tr(brutto_finish_weight_message).arg(tara);
}


QString MainSequence::bruttoFinishMessage(const MifareCardData & bill) const
{
    QString ret;
    ret = tr(brutto_finish_weight_message).arg( bill.memberValue<QString>("bruttoWeight") );
    if ( bill.memberValue<QBitArray>("flags").at(2) ) {
        ret += " " +  tr(brutto_finish_lab_message).arg( bill.memberValue<QString>("pointOfAnal") );
    }
    ret += " " + tr(brutto_finish_bum_message).arg( bill.memberValue<QString>("bum") );

    return ret;
}
#endif


void MainSequence::printOnTablo(const QString & s)
{
    tags[alho_settings.tablo_tag.tag_name]->func(alho_settings.tablo_tag.method_name, this, Q_ARG(const QVariant&, QVariant(s)));
}

#if 0
int MainSequence::getWeight()  throw (MainSequenceException)
{
    QVariant v = tags[alho_settings.weight_tag.tag_name]->func( alho_settings.weight_tag.method_name, this );
    if (!v.isValid()) throw MainSequenceException(tr(weights_dont_work), "weights dont work!");

    return v.toInt();
}
#endif

MainSequence::MainSequence(Tags & t, const QVariantMap& s)
    :init(true),tags(t), app_settings(s), alho_settings(*this, tags) ,on_weight(false), seq_id(0), wake_timer(this)
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

WeighterConf& MainSequence::readStruct(MifareCard & card, MifareCardData& d) throw (MifareCardException, MainSequenceException)
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

void MainSequence::run()
{
    if ( init ) {
        //qDebug() << "\n\n\nmain_sequence!!!!!!!!!!! init begin";
        printOnTablo(tr(greeting_message));
        //qDebug() << "2222";
        setLightsToGreen();
        //qDebug() << "3333";
        init = false;
        //qDebug() << "main_sequence!!!!!!!!!!! init finished\n\n\n\n\n";
        return;
    }


    seqDebug() << "something appeared on weight!!!! id" << seq_id;
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
        MifareCard card(act, alho_settings.reader);

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

            card.autorize(card_code, data_block);

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
/*
void MainSequence::repairBeetFieldCorrectnessIfNeeded(MifareCardData & bill, qx::dao::ptr<t_ttn> ttn) const throw()
{  
    if ( bill.memberValue<uint>("realNumField") == 0 || !wrap_async( [&bill, this]{
                                    return async_func_ptr->async_fetch<t_field>( bill.memberValue<uint>("realNumField") );
                                 } ) )  {

        bill.setMemberValue("realNumField", bill.memberValue<uint>("numField"));
        ttn->real_field = bill.memberValue<uint>("numField");
        //seqDebug () << "field repared: "<<ttn->real_field;
    }
}
*/
#if 0
uint MainSequence::countCarsFromFieldForDayExcludeCurrent(uint ttn_num, uint field_num)  throw()
{
    try {
        QDate workDate = QTime::currentTime().hour()<8 ? QDate::currentDate().addDays(-1) : QDate::currentDate();

        qx_query q("where floor(real_field/100) = "  + QString::number( field_num ) +
                   " and dt_of_brutto>='" + workDate.toString("yyyy.MM.dd") + " 08:00:00'"                            \
                   " and dt_of_brutto<='" + workDate.addDays(1).toString("yyyy.MM.dd") + " 08:00:00' and num_nakl <> " +
                   QString::number(ttn_num) );
        long count = 0;
        async_func_ptr->async_count<t_ttn>( count, q );

        return count;
    }
    catch (MysqlException& ex) {
        seqWarning() << "cant get count of ttns for chemical analysis!!! field: "<<field_num;
    }

    return 0;
}
#endif
#if 0
bool MainSequence::checkForNeedDiscreteAnalisys(long count) const throw()
{
    uint num_in_group = (count + 1) % get_setting<uint>("common_size_of_group", app_settings);
    return num_in_group == get_setting<uint>("common_number_from_group", app_settings);
}

uint MainSequence::getAnalisysPeriodFromStorage(uint typ) const throw(MysqlException, MainSequenceException)
{
    qx::dao::ptr<t_const> const_;

    if ( typ == 0 ) {
        const_ = wrap_async_ex( "error for kontragent type when doint chemical analysis typ=0", QString(),
                                [this]{return async_func_ptr->async_fetch<t_const>(get_setting<QString>("corpotare_check_period_name", app_settings));});
    }
    else if ( typ == 3 ) {
        const_ = wrap_async_ex( "error for kontragent type when doint chemical analysis typ=3", QString(),
                                [this]{return async_func_ptr->async_fetch<t_const>(get_setting<QString>("farmer_check_period_name", app_settings)); });
    }
    else {
        ;
        throw MainSequenceException("error for kontragent type when doint chemical analysis", QString());
    }

    return const_->value.toUInt();
}

bool MainSequence::checkForNeedDatabaseConstAnalisys(long count, long kontrag)  throw ()
{
    if (count + 1 == 1) return true; //first car always go for analysis

    try {
        auto kontr = wrap_async_ex( "cant get kontr of ttns for chemical analysis!!! kontrag: " + QString::number(kontrag),
                                     QString(),
                                    [&kontrag, this]{return async_func_ptr->async_fetch<t_kontr>(kontrag);} );

        uint period = kontr->period;

        if ( period == 0 ) {
            period = wrap_async_ex("error for getting const for type:" + QString::number(kontr->type), QString(),
                                   [&kontr, this]{return getAnalisysPeriodFromStorage(kontr->type);} );
        }

        uint carInPeriod = kontr->car_in_period;
        if (carInPeriod == 0 ) {
            carInPeriod = 1;
            seqWarning() << "error for getting carInPeriod. Kontragent dont have corrent chemical analisys params";
        }

        if ( period == 0 )  {
            period = 1;
            seqWarning() << "error for getting period. Kontragent dont have corrent chemical analisys params";
        }

        uint num_in_group = (count + 1)%period;
        if (num_in_group == 0) num_in_group = period;
        return num_in_group == carInPeriod;
    }
    catch (MainSequenceException& ex) {
        return true;
    }

}
#endif
#if 0
void MainSequence::processChemicalAnalysis(MifareCardData & bill, qx::dao::ptr<t_ttn> ttn ) throw()
{   
    long count   = countCarsFromFieldForDayExcludeCurrent(ttn->num_nakl,
                                                          kontrCodeFromField( bill.memberValue<uint>("realNumField") ) );

    QString alho = get_setting<QString>("common_algorithm_of_analysis", app_settings);

    //seqDebug() << "analisys alho: "<<alho<<" cars_count: "<<count;

    if ( alho == "discrete"  ) {
        if ( !checkForNeedDiscreteAnalisys(count) ) {
            //seqDebug () << " dont needed...";
            return;
        }
    }
    else if (alho == "database_const") {        
        if ( !checkForNeedDatabaseConstAnalisys( count, kontrCodeFromField( bill.memberValue<uint>("realNumField") ) ) ) {
            //seqDebug () << " dont needed...";
            return;
        }
    }

    //определим место отбора
    uint analysis_place = (count+1)%3;

    if (analysis_place==0) analysis_place=3;

    //QBitArray v(3);
    //v.setBit(2, true);
    bill.setMemberValue("flags", 2, true );
    bill.setMemberValue("pointOfAnal", analysis_place);
}
#endif
#if 0
QString MainSequence::getBumsClause(const MifareCardData & bill, qx::dao::ptr<t_cars> car) const throw(MainSequenceException)
{
    QStringList ret;
    if (car->dump_body_truck) {
        ret.append("id = 99");
    }

    if (car->side_board) {
        ret.append("(id % 10) = 2");
    }

    if (car->back_board) {
        qx::dao::ptr<t_const> const_ = wrap_async_ex(tr(get_backboard_bum_weight_const_error), "error getting const for backboard bum by key: " + get_setting<QString>("bum11_name", app_settings),
                        [this]{return async_func_ptr->async_fetch<t_const>(get_setting<QString>("bum11_name", app_settings));});

        if ( bill.memberValue<int>("bruttoWeight") > const_->value.toInt() ) {
            ret.append("id = 11");
        }
        else {
            ret.append("(id % 10) = 1");
        }
    }

    return  ret.join(" or ");
}
#endif
#if 0
void MainSequence::processFreeBum(MifareCardData & bill, qx::dao::ptr<t_ttn> ttn, qx::dao::ptr<t_cars> car) const throw(MainSequenceException)
{
    QString bums_where_clause = getBumsClause(bill, car);

    const QString q1 =  "select * from t_bum where state=1 and queue=(select min(queue) from t_bum where state=1 and ("+bums_where_clause+")) and ("+bums_where_clause +");";
    const QString q2 =  "select * from t_bum where queue=(select min(queue) from t_bum where ("+bums_where_clause+")) and ("+bums_where_clause +");";

    qx::dao::ptr<t_bum> bum = wrap_async_ex(tr(get_free_bum_error), "Error getting free bum1",
        [&bums_where_clause, &q1, this]{ return async_func_ptr->async_exec_query<t_bum>(q1, false);});



    if ( !bum ) {

        bum = wrap_async_ex(tr(get_free_bum_error), "Error getting free bum2",
            [&bums_where_clause, &q2, this]{ return async_func_ptr->async_exec_query<t_bum>(q2);});
    }


    bum->queue += 1;



    wrap_async_ex( tr(update_bum_queue_error), "Error updating bum queue", [&bum, this]{ async_func_ptr->async_update(bum); });

    if ( bum->id != 99 ) {
        ttn->bum_platforma = bum->id % 10;
        ttn->bum           = bum->id / 10;
        //wrap_async_ex( update_ttn_platform_error, "Error updating ttn platform", [&ttn]{ async_update(ttn); });
        bill.setMemberValue("bum", ttn->bum);
    }
    else {
        ttn->bum           = bum->id;

        bill.setMemberValue("bum", bum->id);
    }
}
#endif

#if 0
void MainSequence::updateBruttoValues(MifareCardData& bill, qx::dao::ptr<t_ttn> ttn, const MifareCard& card) const throw(MainSequenceException)
{    
    ttn->real_field       = bill.memberValue<int>("realNumField");
    ttn->loader           = bill.memberValue<int>("numLoader");
    ttn->dt_of_load       = bill.memberValue<QDateTime>("dateOfLoad");
    ttn->brutto           = bill.memberValue<int>("bruttoWeight");
    ttn->dt_of_brutto     = bill.memberValue<QDateTime>("dateOfBrutto");
    ttn->driver           = bill.memberValue<int>("driver");//carCodeFromDriver( memberValue<int>("driver", bill) ) ;
    ttn->bum              = bill.memberValue<int>("bum");
    ttn->routed_to_lab    = bill.memberValue<QBitArray>("flags").at(2);
    ttn->num_kart         = byteArrayToString (card.uid());
    ttn->copy             = 0;
    ttn->time_of_brutto   = ttn->dt_of_brutto.time().toString("hh:mm:ss");
    ttn->brutto_platforma = seq_id;


    wrap_async_ex( tr(update_ttn_error_message), "Error updating ttn brutto", [&ttn, this]{ async_func_ptr->async_update(ttn); });
}
#endif
#if 0
void MainSequence::updateTaraValues(MifareCardData& bill, qx::dao::ptr<t_ttn> ttn, qx::dao::ptr<t_cars> car, bool pure_weight) const throw (MainSequenceException)
{
    if ( pure_weight ) {
        ttn->real_bum      = bill.memberValue<int>("bumFact");
        ttn->kagat         = bill.memberValue<int>("kagat");
        ttn->dt_of_unload  = bill.memberValue<QDateTime>("dateOfUnload");
        ttn->was_in_lab    = bill.memberValue<QBitArray>("flags").at(3);

       // seqDebug () << "bum: " << ttn->bum;
    }

    ttn->copy          = 0;
    ttn->time_of_tara  = ttn->dt_of_tara.time().toString("hh:mm:ss");
    ttn->tara_platforma  = seq_id;
    ttn->field_from_car  = car->num_field;

    //seqDebug () << "real_rup_tara: " << ttn->real_rup_tara;

    wrap_async_ex( tr(update_ttn_error_message),
                                   "Error updating ttn tara: ttn_num: " + QString::number(ttn->num_nakl), [&ttn, this]{ async_func_ptr->async_update(ttn); });
}
#endif
#if 0
bool MainSequence::isPureBruttoWeight(const MifareCardData& bill) const throw (MainSequenceException)
{
    if ( bill.memberValue<uint>("bruttoWeight") == 0 ) return true;
    if ( bill.memberValue<uint>("kagat") == 0 ) return false;

    throw MainSequenceException(tr(confuse_brutto_tara_error_message), "confused brutto with tara");
}
#endif
#if 0
bool MainSequence::checkDeltaForReweights(int prev_weight, int weight) const
{
    return qAbs( prev_weight - weight ) < get_setting<int>("brutto_delta_between_reweights", app_settings);
}
#endif
#if 0
bool MainSequence::isWeightCorrect(int w)
{
    return w >= 0 && tags[alho_settings.weight_stable.tag_name]->func(alho_settings.weight_stable.method_name, this).toBool();
}
#endif

#if 0
void MainSequence::brutto(MifareCardData & bill, BaseWeighter::Pointer weighter)  throw(MainSequenceException)
{


    /*auto ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
                                  [&bill, this]{return async_func_ptr->async_fetch<t_ttn>(bill["billNumber"].toUInt());});
    */

    /* repairBeetFieldCorrectnessIfNeeded(bill, ttn); */

    int weight = getWeight();

    checkWeightCorrect( weight );



    if ( !weighter->isPureBruttoWeight(bill) ){
        /*
        if ( !checkDeltaForReweights(bill.memberValue<uint>("bruttoWeight"), weight) )  {
            throw MainSequenceException(tr(brutto_rupture_failed_message),
                                    "reweight: brutto rup failed!: prevWeight: " +
                                     bill.memberValue<QString>("bruttoWeight")  +
                                    " cur: " + QString::number(weight)  + " max_delta: " +
                                    get_setting<QString>("brutto_delta_between_reweights", app_settings));
        }

        bill.setMemberValue("flags", 0, true );
        bill.setMemberValue("bruttoWeight", weight);
        bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());
        */
        weighter->reBrutto(weight, bill );

    }
    else {
        //seqDebug() << "weight!";

        /*bill.setMemberValue("bruttoWeight", weight);
        bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());

        processChemicalAnalysis( bill, ttn );
        processFreeBum( bill, ttn, car );
        */
  //      qDebug() << "!!!!!!!!44444444444444";
        weighter->brutto(weight, bill);
  //      qDebug() << "!!!!!!!!5555555555555555";
    }


    //updateBruttoValues(bill, ttn, card);

}

#endif

#if 0
bool MainSequence::tara(MifareCardData & bill, BaseWeighter::Pointer weighter)  throw (MainSequenceException)
{
    int weight = getWeight();

    if ( !isWeightCorrect( weight ) ) {
        throw MainSequenceException(tr(weight_not_stable_message), "tara: weights dont stable!" );
    }

   // qx::dao::ptr<t_ttn> ttn;



    if ( !weighter->isPureTaraWeight(bill) ) {
/*        seqWarning()<< "tara reweiting!";

        ttn = ttnByDriver( carCodeFromDriver( bill.memberValue<uint>("driver") ) );

        if (!checkDeltaForReweights(ttn->tara, weight)) {
            throw MainSequenceException(tr(tara_rupture_failed_message),
                                    "reweight: tara rup failed!: prevWeight: " +
                                    QString::number(ttn->tara)  + " cur: " + QString::number(weight)  + " max_delta: " +
                                    get_setting<QString>("brutto_delta_between_reweights", app_settings));
        }
        ttn->tara = weight;
        ttn->dt_of_tara = QDateTime::currentDateTime();

        processDrivingTime(ttn, car);

        repairBumCorrectnessIfNeeded(ttn);

        processTaraRupture(ttn, car);
*/
        weighter->reTara(weight, bill);

        //updateTaraValues(bill, ttn, car, false);
        //need_new = false;
        //return ttn, qx::dao::ptr<t_ttn>()};

        //printFinishReport(ttn, car);
        return false;
    }
    else  {
        /*seqDebug( ) << "tara weight!!!";

        ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
                                  [&bill, this]{return async_func_ptr->async_fetch<t_ttn>(bill["billNumber"].toUInt());});

        ttn->tara = weight;
        ttn->dt_of_tara = QDateTime::currentDateTime();

        checkBum(bill);
        checkLaboratory(bill, car);
        checkKagat(bill);

        clearBumQueue(ttn);

        processDrivingTime(ttn, car);

        repairBumCorrectnessIfNeeded(ttn);

        processTaraRupture(ttn, car);

        */
        weighter->tara(weight, bill);
        //updateTaraValues(bill, ttn, car, true);



        //bill.clear();

       // need_new = true;
        return true;
    }

    //return ttn;
}

#endif

#if 0
void MainSequence::clearBumQueue(qx::dao::ptr<t_ttn> ttn) const throw(MainSequenceException)
{
    int bum = ttn->bum;

    if (bum != 99) {
        bum = bum * 10 + ttn->bum_platforma;
    }

    wrap_async_ex(tr(clear_bum_queue_error), "clear bum queue error",
                [&ttn, this, &bum]{ async_func_ptr->async_call_query("update t_bum set queue = GREATEST(queue - 1, 0) where id=" + QString::number(bum) + ";") ;});

}
#endif

#if 0
qx::dao::ptr<t_ttn> MainSequence::ttnByDriver( int drv ) throw (MainSequenceException)
{
//    auto ret = async_func_ptr->wrap_async_ex(tr(get_ttn_by_driver_tara0_error_message), "getting ttn by driver with zero tara failed: " + QString::number(drv),
//                            [&drv, this]{ return async_func_ptr->async_fetch_by_query<t_ttn>(
//                                qx_query().where("driver").isEqualTo(drv).and_("brutto").isNotEqualTo(0).and_("tara").isEqualTo(0)); });

        auto ret = wrap_async_ex(tr(get_ttn_by_driver_tara0_error_message), "getting ttn by driver with zero tara failed: " + QString::number(drv),
                                [&drv, this]{ return async_func_ptr->async_exec_query<t_ttn>(
                                                     "select * from t_ttn where driver="+QString::number(drv) + " and brutto != 0 and tara=0", false);});

    if ( ret ) {
    seqWarning() << "Corrupted data on card!. Maybe dispatcher made task before tara finished! drv: " + QString::number(drv) <<" founded ttn: " + QString::number(ret->num_nakl);
        return ret;
    }

/*    ret = async_func_ptr->wrap_async_ex(tr(get_last_ttn_by_driver_error_message), "getting last ttn by driver failed! drv: " + QString::number(drv),
                                [&drv, this]{ return async_func_ptr->async_fetch_by_query<t_ttn>(
                                qx_query().where("driver").isEqualTo(drv).and_("dt_of_tara").
                                    in_Select("select MAX(dt_of_tara) from t_ttn where driver=" + QString::number(drv)));});
*/


    const QString qq = "select * from t_ttn where driver = " + QString::number(drv)
            + " and dt_of_tara in (select MAX(dt_of_tara) from t_ttn where driver=" + QString::number(drv) + ")";

    ret = wrap_async_ex(tr(get_last_ttn_by_driver_error_message), "getting last ttn by driver failed! drv: " + QString::number(drv),
                                [&drv, this, &qq]{ return async_func_ptr->async_exec_query<t_ttn>(qq);});


    return ret;
}
#endif

#if 0
bool MainSequence::isPureTaraWeight(const MifareCardData &bill) const throw(MainSequenceException)
{
    if ( bill.memberValue<int>("bruttoWeight") > 0 ) return true;
    if ( bill.memberValue<int>("realNumField") == 0 ) return false;

    throw MainSequenceException(tr(forget_brutto_on_tara_error_message), "forget for brutto!" );
}
#endif

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
#if 0
void MainSequence::checkBum( MifareCardData& bill )const throw(MainSequenceException)
{
    if ( bill.memberValue<int>("bum") == 99 && bill.memberValue<int>("bumFact") == 0 ) {
        bill.setMemberValue("bumFact", 99);
        bill.setMemberValue("kagat", 99);
        //wrap_async_ex( update_bum_queue_error, "Error updating bum queue", [&bum, this]{ async_update(bum); });
    }
}

void MainSequence::checkLaboratory( const MifareCardData& bill , qx::dao::ptr<t_cars> car)const throw(MainSequenceException)
{
    if ( bill.memberValue<QBitArray>("flags").at(2) && !bill.memberValue<QBitArray>("flags").at(3) ) {
        car->block = 1;
        wrap_async_ex( tr(blocking_car_for_lab_error), "Error blocking car which wasnt in lab", [&car, this]{ async_func_ptr->async_update(car); });

        throw MainSequenceException (tr(car_dont_was_in_lab), "car dont was in lab");
    }
}

void MainSequence::checkKagat(const MifareCardData& bill)const throw(MainSequenceException)
{
    if ( bill.memberValue<int>("kagat") ==0 )
        throw MainSequenceException(tr(car_has_not_been_unloaded), "car was not unloaded!");

    qx::dao::ptr<t_kagat> kagat = wrap_async_ex(tr(getting_kagat_error), "cant get kagat: " + bill.memberValue<QString>("kagat"),
                    [&bill, this]{ return async_func_ptr->async_fetch<t_kagat>(bill.memberValue<int>("kagat")) ;});

    if (!kagat->state) {
        throw MainSequenceException(tr(kagat_was_closed_error), "kagat " + bill.memberValue<QString>("kagat") + " was closed!");
    }

}
#endif

#if 0
void MainSequence::processDrivingTime(qx::dao::ptr<t_ttn> ttn, qx::dao::ptr<t_cars> car) const throw (MainSequenceException)
{
    ushort rup      = dateTimeToTimeShit(ttn->time_return) - dateTimeToTimeShit(ttn->date_time) / 60; /*to hours*/

    if ( car->vremja_na_hodku > rup ) {
        ttn->time_return = timeShitToDateTime( dateTimeToTimeShit(ttn->date_time) + car->vremja_na_hodku * 60 );
    }
}
#endif
#if 0
bool MainSequence::printStartReport(const qx::dao::ptr<t_ttn> & ttn, const qx::dao::ptr<t_cars> & car) const throw(MainSequenceException)
{
    ReportsManager m(app_settings, *async_func_ptr, *convience_func_ptr);

    qx::dao::ptr<t_field> field = wrap_async_ex(tr(cant_get_field_when_printing), "startReport: cant get field " + QString::number(ttn->field) + " when printing for passed ttn: " + QString::number(ttn->num_nakl),
                                [&ttn, this]{return async_func_ptr->async_fetch<t_field>( ttn->field ); });

    return m.printReport(ttn, car, field, get_setting<QString>("start_report_file_name", app_settings));
}
#endif

#if 0
bool MainSequence::printFinishReport( const qx::dao::ptr<t_ttn>& ttn, const qx::dao::ptr<t_cars>& car) const throw(MainSequenceException)
{
    ReportsManager m(app_settings, *async_func_ptr, *convience_func_ptr);

    qx::dao::ptr<t_field> field = wrap_async_ex(tr(cant_get_field_when_printing), "finishReport: cant get field " + QString::number(ttn->real_field) + " when printing for passed ttn: " + QString::number(ttn->num_nakl),
                                [&ttn, this]{return async_func_ptr->async_fetch<t_field>( ttn->real_field ); });

    return m.printReport(ttn, car, field, get_setting<QString>("finish_report_file_name", app_settings));

}
#endif

#if 0
void MainSequence::repairBumCorrectnessIfNeeded(qx::dao::ptr<t_ttn> ttn) const throw (MainSequenceException)
{
    if ( ttn->bum == ttn->real_bum ) return;

    int bum = 99;
    if (ttn->bum != 99)
        bum = ttn->bum * 10 + ttn->bum_platforma;

    if ( !checkBumWorks(ttn->dt_of_brutto, ttn->dt_of_tara, bum) ) {
        ttn->bum = ttn->real_bum;
    }
}


bool MainSequence::checkBumWorks(const QDateTime & date_from, const QDateTime & date_to, long bum) const throw(MainSequenceException)
{
    //qx_query q;
    //q.where("date_time").isGreaterThanOrEqualTo(date_from).and_("date_time").isLessThanOrEqualTo(date_to).and_("bum").isEqualTo(QVariant::fromValue<long>(bum));


    const QString q = "select * from t_bum_state_log where date_time >= '" + date_from.toString("yyyy.MM.dd hh:mm:ss") + "' and date_time <= '" + date_to.toString("yyyy.MM.dd hh:mm:ss") + "' and bum = " + QString::number(bum);
    qx::dao::ptr<t_bum_state_log> bum_log = wrap_async_ex( tr(cant_get_bum_state_log_message), "cant get bum state log",
        [&q, this]{ return async_func_ptr->async_exec_query<t_bum_state_log>(q, false) ;});



    if (bum_log) return false;

    qx::dao::ptr<t_bum> bum_ptr = wrap_async_ex(tr(cant_get_bum_message), "cant get bum: " + QString::number(bum),
                        [&bum, this]{ return async_func_ptr->async_fetch<t_bum>(bum);});

    return static_cast<bool> (bum_ptr->state);
}
#endif

#if 0
void MainSequence::processTaraRupture(qx::dao::ptr<t_ttn> ttn, qx::dao::ptr<t_cars> car) const throw(MainSequenceException)
{
    QString percent = convience_func_ptr->getConst( get_setting<QString>("tara_percent_name", app_settings) )->value ;

    QString count;
    if ( car->amount_of_car_for_middle_tara < 0 ) {
        count = convience_func_ptr->getConst( get_setting<QString>("tara_cars_mid_count_name", app_settings) )->value ;
    }
    else {
        count = QString::number( car->amount_of_car_for_middle_tara );
    }






    int mid_tara = wrap_async_ex( tr(error_getting_mid_tara_message), "error getting mid tara",
                                [this, &ttn, &car, &count]{  return async_func_ptr->async_call_query<int>(
                                    "select avg(tara) from (select tara from t_ttn where tara!=0 and num_nakl!="+
                                    QString::number(ttn->num_nakl)+
                                    " and car="+QString::number(car->id)+
                                    " order by -dt_of_tara limit "+count+") as temp_table;");});




    ttn->rup_tara           = mid_tara * percent.toUInt() / 100;
    ttn->real_rup_tara      = qAbs(mid_tara - static_cast<int>(ttn->tara));

    //seqDebug () << "mid_tara: "<<mid_tara;
}
#endif



/*
qx::dao::ptr<t_ttn> MainSequence::makeNewTask(qx::dao::ptr<t_cars> car, const MifareCardData& bill) const throw (MainSequenceException)
{
    if (car->num_field == 0) return qx::dao::ptr<t_ttn>();

    QDateTime end_time = timeShitToDateTime( car->vremja_na_hodku*60 + dateTimeToTimeShit(QDateTime::currentDateTime())  );

    qx::dao::ptr<t_ttn> ttn = qx::dao::ptr<t_ttn>(new t_ttn);

    ttn->date_time      = QDateTime::currentDateTime();
    ttn->car            = car->id;
    ttn->field          = car->num_field;
    ttn->driver         = bill.memberValue<uint>("driver");
    ttn->time_return    = end_time;
    ttn->copy           = 0;
    ttn->time_of_return = end_time.toString("hh:mm:ss");
    ttn->loader         = car->num_loader;

    //qx::dao::insert(ttn, &database);

    wrap_async_ex(tr(error_make_new_task), "error make new task", [this, &ttn]{return async_func_ptr->async_insert(ttn);});

    return ttn;
}
*/

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

#if 0
BaseWeighter::Pointer MainSequence::createWeighter(const MifareCardData& bill) throw (MainSequenceException)
{
    int material_id = bill.memberValue<int>( "material" );
    int num_nakl    = bill.memberValue<int>("billNumber");

    QVariantList weighters_conf = get_setting<QVariantList>("weighters_conf", app_settings );

    for (int i = 0; i<weighters_conf.count(); ++i) {
        bool ok = false;
        if ( weighters_conf[i].toMap()["material"].toInt(&ok)==material_id ) {
            if (!ok)
                throw MainSequenceException("corrupted material info",
                  "corrupted material info ttn: " + QString::number(num_nakl) );
            QString weighter_name = weighters_conf[i].toMap()["weighter"].toString();
            qDebug() << "weighter: " << weighter_name;
            return BaseWeighter::create(weighter_name, *this);
        }
    }

    throw MainSequenceException("cant find weighter", "cant find weighter ttn: " + QString::number(num_nakl) );
}




#endif
