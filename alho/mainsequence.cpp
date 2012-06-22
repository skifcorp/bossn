#include "mainsequence.h"
#include "mifarecard.h"
#include "settingstool.h"
#include "func.h"

#include "dbstructs.h"
#include "warnmessages.h"
#include "reportsmanager.h"
#include "codeshacks.h"
#include "conviencefuncs.h"

#include <QBitArray>



const QString brutto_finish_weight_message                  = QT_TRANSLATE_NOOP("MainSequence", "%1 kg");
const QString brutto_finish_lab_message                     = QT_TRANSLATE_NOOP("MainSequence", "Lab(%1)");
const QString brutto_finish_bum_message                     = QT_TRANSLATE_NOOP("MainSequence", "BUM %1");



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

    alho_settings.reader.name           = get_setting<QString>("reader_tag", s);
    alho_settings.reader.do_on          = get_setting<QString>("reader_do_on", s);
    alho_settings.reader.do_off         = get_setting<QString>("reader_do_off", s);
    alho_settings.reader.activate_idle  = get_setting<QString>("reader_activate_idle", s);
    alho_settings.reader.host_coded_key = get_setting<QString>("reader_host_coded_key", s);
    alho_settings.reader.do_auth        = get_setting<QString>("reader_do_auth", s);
    alho_settings.reader.write_block    = get_setting<QString>("reader_write_block", s);
    alho_settings.reader.read_block     = get_setting<QString>("reader_read_block", s);
    alho_settings.reader.do_sound       = get_setting<QString>("reader_sound", s);
    seq_id                              = get_setting<int>("id", s);

    database = QSqlDatabase::addDatabase(get_setting<QString>("database_driver", s), "sequ_" + QString::number(seq_id));

    database.setHostName(get_setting<QString>("database_host", s));
    database.setDatabaseName(get_setting<QString>("database_name", s));
    database.setUserName(get_setting<QString>("database_user", s));
    database.setPassword(get_setting<QString>("database_password", s));
    database.setConnectOptions( get_setting<QString>("connection_options", s, QString() ));

    async_func_ptr     = async_func_ptr_t( new  async_func(database, *this) );
    convience_func_ptr = convience_func_ptr_t( new convience_func (*async_func_ptr) );

    printOnTablo(greeting_message);
    setLightsToGreen();

//    t_ttn ttn;
//    ttn.bum = 12;
//    ttn.car = 120;
//    qx::dao::insert( ttn, &database, "t_ttn", true );


}

QString MainSequence::taraFinishMessage() const
{
    return "tara finished!";
}


QString MainSequence::bruttoFinishMessage(const QVariantMap & bill) const
{
    QString ret;
    ret = brutto_finish_weight_message.arg( memberValue<QString>("bruttoWeight", bill) );
    if ( memberValue<QBitArray>("flags", bill).at(2) ) {
        ret += " " +  brutto_finish_lab_message.arg( memberValue<QString>("pointOfAnal", bill) );
    }
    ret += " " + brutto_finish_bum_message.arg( memberValue<QString>("bum", bill) );

    return ret;
}

void MainSequence::printOnTablo(const QString & s) const
{
    tags[alho_settings.tablo_tag.tag_name]->func(alho_settings.tablo_tag.method_name, Q_ARG(const QVariant&, QVariant(s)));
}

int MainSequence::getWeight() const throw (MainSequenceException)
{
    QVariant v = tags[alho_settings.weight_tag.tag_name]->func( alho_settings.weight_tag.method_name );
    if (!v.isValid()) throw MainSequenceException(weights_dont_work, "weights dont work!");

    return v.toInt();
}


MainSequence::MainSequence(Tags & t, const QVariantMap& s):tags(t), app_settings(s), on_weight(false), seq_id(0), wake_timer(this)
{
    qx::QxSqlDatabase::getSingleton()->setTraceSqlQuery(false);
    qx::QxSqlDatabase::getSingleton()->setTraceSqlRecord(false);

    connect(&wake_timer, SIGNAL(timeout()), this, SLOT(wakeUp()));
    wake_timer.setSingleShot(true);
}

QString MainSequence::detectPlatformType(const QVariantMap & bill) const throw (MainSequenceException)
{
    QString typ = get_setting<QString>("platform_type", app_settings);
    if (typ != "auto") return typ;

    if (!checkMember("bruttoWeight", bill, 0) &&
        !checkMember("dateOfBrutto", bill, timeShitToDateTime(0)))
        return "brutto";

    if ( !checkMember("bumFact", bill, 0) &&
         !checkMember("kagat", bill, 0) &&
         !checkMember("taraWeight", bill, 0) &&
         !checkMember("dateOfTara", bill, timeShitToDateTime(0))) {
        return "brutto"; /*reweighting*/
    }

    if ( checkMember("bumFact", bill, 0) &&
         checkMember("kagat", bill, 0) ) {
        return "tara";
    }

    throw MainSequenceException(autodetect_platform_type_error_message ,"something terrible happens!!! cant detect platform type. Maybe bill corrupted :(" );
}

void MainSequence::checkForStealedCard(const ActivateCardISO14443A& prev_card, const ActivateCardISO14443A& card) const throw (MainSequenceException)
{
    if ( prev_card.uid.isEmpty() ) return;

    if ( prev_card.uid == card.uid ) return;

    throw MainSequenceException(stealed_card_message, "You used stealed card!");
}


void MainSequence::wakeUp()
{
    cont();
}

void MainSequence::onAppearOnWeight(const QString& )
{
    //qDebug ( ) << "status: " << status();

    if ( status() == NotStarted || status() == Terminated ) {
        restart();

        wakeUp();
    }
}

void MainSequence::run()
{
    seqDebug() << "something appeared on weight!!!! id" << seq_id;
    on_weight = true;

    printOnTablo(apply_card_message);

    setLightsToRed();


    //tags["reader1"]->func("doOn");
    tags[ alho_settings.reader.name]->func( alho_settings.reader.do_on );

    QByteArray card_code = get_setting<QByteArray>("card_code" , app_settings);
    uint data_block      = get_setting<uint>      ("data_block", app_settings);

    ActivateCardISO14443A cur_act;


    while(on_weight) {
        long cur_num_nakl = 0;

        ActivateCardISO14443A act = tags[alho_settings.reader.name]->func(alho_settings.reader.activate_idle).value<ActivateCardISO14443A>();
        MifareCard card(tags[alho_settings.reader.name], act, alho_settings.reader);

        if ( !card.active() ) {
            seqDebug() << "card not active!!!";
            sleepnbtm(); continue;
        }

        //seqDebug()<< "card active! platform: " + QString::number(seq_id);


        try {
            printOnTablo(processing_message);

            if ( !database.isOpen() ) {
                if ( !async_func_ptr->async_call( [this]{return database.open();}) )
                    throw MainSequenceException(error_database_lost, "Error database lost!!!");
            }

            checkForStealedCard(cur_act, act); cur_act = act;

            card.autorize(card_code, data_block);

            processPerimeter();

            QVariantMap bill = card.readStruct(bill_conf(app_settings));

            if (!checkMember("billNumber", bill, 0) || !checkMember("driver", bill, 0)) {
                throw MainSequenceException(card_is_empty_error_message, "bill is empty");
            }

            cur_num_nakl = memberValue<int>("billNumber", bill);

            auto car = async_func_ptr->wrap_async_ex(fetch_car_error_message, "fetching car failed!!!: driver: " + memberValue<QString>("driver", bill),
                                  [&bill, this]{return async_func_ptr->async_fetch<t_cars>( carCodeFromDriver( memberValue<uint>("driver", bill) ) );});


            if (car->block) {
                throw MainSequenceException(car_blocked_message, "car is blocked!!!");
            }

            QString platform_type = detectPlatformType(bill);

            if (platform_type == "brutto" ) {
                seqDebug() << "brutto...";

                brutto(bill, car, card);

                setMemberValue("bumFact", 8, bill);
                setMemberValue("kagat"  , 1, bill);
                setMemberValue("flags"  , 3, 1, bill);

                card.writeStruct(bill_conf(app_settings), bill);

                printOnTablo( bruttoFinishMessage(bill) );

                //tags[alho_settings.reader.name]->func(alho_settings.reader.do_sound, Q_ARG(QVariant, get_setting<int>("beep_length", app_settings)));

                sleepnb( get_setting<int>("brutto_finish_pause", app_settings) );
                printOnTablo( apply_card_message );
                continue;
            }
            else if (platform_type == "tara" ) {
                seqDebug() << "tara...";

                tara(bill, car);
                card.writeStruct(bill_conf(app_settings), bill);
                printOnTablo( taraFinishMessage() );

                //tags[alho_settings.reader.name]->func(alho_settings.reader.do_sound, Q_ARG(QVariant, get_setting<int>("beep_length", app_settings)));

                sleepnb( get_setting<int>("brutto_finish_pause", app_settings) );
                printOnTablo( apply_card_message );
                continue;
            }
        }
        catch (MifareCardAuthException& ex) {
            seqWarning() << "auth_exeption! "<<ex.message() << " curNumNakl: " << cur_num_nakl;

            sleepnbtmerr(card_autorize_error_message, apply_card_message);
            continue;
        }
        catch (MifareCardException& ex) {
            seqWarning() << "card_exception! "<<ex.message() << " curNumNakl: "<< cur_num_nakl;
            sleepnbtmerr(ex.message(), apply_card_message);
            continue;
        }
        catch (MainSequenceException& ex) {
            if ( database.isOpenError() ) {
                seqDebug() << "---------->>> OPEN ERROR!!!! isOpen: " << database.isOpen();
            }

            seqWarning()<<"sequence_exception: " << ex.adminMessage() << " curNumNakl: "<< cur_num_nakl;;

            sleepnbtmerr(ex.userMessage(), apply_card_message);
            continue;
        }
    }

    seqDebug () << "\n\nexit from onAppearOnWeight!!!!!!!";

    printOnTablo(greeting_message);
    setLightsToGreen();
}

void MainSequence::repairBeetFieldCorrectnessIfNeeded(QVariantMap & bill, qx::dao::ptr<t_ttn> ttn) const throw()
{  
    if ( memberValue<uint>("realNumField", bill) == 0 || !async_func_ptr->wrap_async( [&bill, this]{
                                    return async_func_ptr->async_fetch<t_field>( memberValue<uint>("realNumField", bill) );
                                 } ) )  {

        setMemberValue("realNumField", memberValue<uint>("numField", bill), bill);
        ttn->real_field = memberValue<uint>("numField", bill);
        //seqDebug () << "field repared: "<<ttn->real_field;
    }
}


uint MainSequence::countCarsFromFieldForDayExcludeCurrent(uint ttn_num, uint field_num) const throw()
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


bool MainSequence::checkForNeedDiscreteAnalisys(long count) const throw()
{
    uint num_in_group = (count + 1) % get_setting<uint>("common_size_of_group", app_settings);
    return num_in_group == get_setting<uint>("common_number_from_group", app_settings);
}

uint MainSequence::getAnalisysPeriodFromStorage(uint typ) const throw(MysqlException, MainSequenceException)
{
    qx::dao::ptr<t_const> const_;

    if ( typ == 0 ) {
        const_ = async_func_ptr->wrap_async_ex( "error for kontragent type when doint chemical analysis typ=0", QString(),
                                [this]{return async_func_ptr->async_fetch<t_const>(get_setting<QString>("corpotare_check_period_name", app_settings));});
    }
    else if ( typ == 3 ) {
        const_ = async_func_ptr->wrap_async_ex( "error for kontragent type when doint chemical analysis typ=3", QString(),
                                [this]{return async_func_ptr->async_fetch<t_const>(get_setting<QString>("farmer_check_period_name", app_settings)); });
    }
    else {
        ;
        throw MainSequenceException("error for kontragent type when doint chemical analysis", QString());
    }

    return const_->value.toUInt();
}

bool MainSequence::checkForNeedDatabaseConstAnalisys(long count, long kontrag) const throw ()
{
    if (count + 1 == 1) return true; //first car always go for analysis

    try {
        auto kontr = async_func_ptr->wrap_async_ex( "cant get kontr of ttns for chemical analysis!!! kontrag: " + QString::number(kontrag),
                                     QString(),
                                    [&kontrag, this]{return async_func_ptr->async_fetch<t_kontr>(kontrag);} );

        uint period = kontr->period;

        if ( period == 0 ) {
            period = async_func_ptr->wrap_async_ex("error for getting const for type:" + QString::number(kontr->type), QString(),
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


void MainSequence::processChemicalAnalysis(QVariantMap & bill, qx::dao::ptr<t_ttn> ttn )const throw()
{   
    long count   = countCarsFromFieldForDayExcludeCurrent(ttn->num_nakl,
                                                          kontrCodeFromField( memberValue<uint>("realNumField", bill) ) );

    QString alho = get_setting<QString>("common_algorithm_of_analysis", app_settings);

    //seqDebug() << "analisys alho: "<<alho<<" cars_count: "<<count;

    if ( alho == "discrete"  ) {
        if ( !checkForNeedDiscreteAnalisys(count) ) {
            //seqDebug () << " dont needed...";
            return;
        }
    }
    else if (alho == "database_const") {        
        if ( !checkForNeedDatabaseConstAnalisys( count, kontrCodeFromField( memberValue<uint>("realNumField", bill) ) ) ) {
            //seqDebug () << " dont needed...";
            return;
        }
    }

    //определим место отбора
    uint analysis_place = (count+1)%3;

    if (analysis_place==0) analysis_place=3;

    //QBitArray v(3);
    //v.setBit(2, true);
    setMemberValue("flags", 2, true, bill );
    setMemberValue("pointOfAnal", analysis_place, bill);
}

QString MainSequence::getBumsClause(const QVariantMap & bill, qx::dao::ptr<t_cars> car) const throw(MainSequenceException)
{
    QStringList ret;
    if (car->dump_body_truck) {
        ret.append("id = 99");
    }

    if (car->side_board) {
        ret.append("(id % 10) = 2");
    }

    if (car->back_board) {
        qx::dao::ptr<t_const> const_ = async_func_ptr->wrap_async_ex(get_backboard_bum_weight_const_error, "error getting const for backboard bum",
                        [this]{return async_func_ptr->async_fetch<t_const>(get_setting<QString>("bum11_name", app_settings));});

        if ( memberValue<int>("bruttoWeight", bill) > const_->value.toInt() ) {
            ret.append("id = 11");
        }
        else {
            ret.append("(id % 10) = 1");
        }
    }

    return  ret.join(" or ");
}

void MainSequence::processFreeBum(QVariantMap & bill, qx::dao::ptr<t_ttn> ttn, qx::dao::ptr<t_cars> car) const throw(MainSequenceException)
{
    QString bums_where_clause = getBumsClause(bill, car);

    const QString q1 =  "select * from t_bum where state=1 and queue=(select min(queue) from t_bum where state=1 and ("+bums_where_clause+")) and ("+bums_where_clause +");";
    const QString q2 =  "select * from t_bum where queue=(select min(queue) from t_bum where ("+bums_where_clause+")) and ("+bums_where_clause +");";

    qx::dao::ptr<t_bum> bum = async_func_ptr->wrap_async_ex(get_free_bum_error, "Error getting free bum1",
        [&bums_where_clause, &q1, this]{ return async_func_ptr->async_exec_query<t_bum>(q1, false);});



    if ( !bum ) {
        //qDebug () << "BUM IS ENPTYYYYYYYYYYYYYYYY!";

        bum = async_func_ptr->wrap_async_ex(get_free_bum_error, "Error getting free bum2",
            [&bums_where_clause, &q2, this]{ return async_func_ptr->async_exec_query<t_bum>(q2);});
    }

    //qDebug () << "q1: " << q1 << " q2: " <<q2;

    //seqDebug () <<  "bum queue1: " << bum->queue << "id: " << bum->id << " state: " << bum->state;

    bum->queue += 1;

    //seqDebug () <<  "bum queue2: " << bum->queue;

    async_func_ptr->wrap_async_ex( update_bum_queue_error, "Error updating bum queue", [&bum, this]{ async_func_ptr->async_update(bum); });

    if ( bum->id != 99 ) {
        ttn->bum_platforma = bum->id % 10;
        ttn->bum           = bum->id / 10;
        //wrap_async_ex( update_ttn_platform_error, "Error updating ttn platform", [&ttn]{ async_update(ttn); });
        setMemberValue("bum", ttn->bum, bill);
    }
    else {
        ttn->bum           = bum->id;

        setMemberValue("bum", bum->id, bill);
    }
}

void MainSequence::updateBruttoValues(QVariantMap& bill, qx::dao::ptr<t_ttn> ttn, const MifareCard& card) const throw(MainSequenceException)
{    
    ttn->real_field       = memberValue<int>("realNumField", bill);
    ttn->loader           = memberValue<int>("numLoader", bill);
    ttn->dt_of_load       = memberValue<QDateTime>("dateOfLoad", bill);
    ttn->brutto           = memberValue<int>("bruttoWeight", bill);
    ttn->dt_of_brutto     = memberValue<QDateTime>("dateOfBrutto", bill);
    ttn->driver           = memberValue<int>("driver", bill);//carCodeFromDriver( memberValue<int>("driver", bill) ) ;
    ttn->bum              = memberValue<int>("bum", bill);
    ttn->routed_to_lab    = memberValue<QBitArray>("flags", bill).at(2);
    ttn->num_kart         = byteArrayToString (card.uid());
    ttn->copy             = 0;
    ttn->time_of_brutto   = ttn->dt_of_brutto.time().toString("hh:mm:ss");
    ttn->brutto_platforma = seq_id;


    async_func_ptr->wrap_async_ex( update_ttn_error_message, "Error updating ttn brutto", [&ttn, this]{ async_func_ptr->async_update(ttn); });
}

void MainSequence::updateTaraValues(QVariantMap& bill, qx::dao::ptr<t_ttn> ttn, qx::dao::ptr<t_cars> car, bool pure_weight) const throw (MainSequenceException)
{
    if ( pure_weight ) {
        ttn->real_bum      = memberValue<int>("bumFact", bill);
        ttn->kagat         = memberValue<int>("kagat", bill);
        ttn->dt_of_unload  = memberValue<QDateTime>("dateOfUnload", bill);
        ttn->was_in_lab    = memberValue<QBitArray>("flags", bill).at(3);

       // seqDebug () << "bum: " << ttn->bum;
    }

    ttn->copy          = 0;
    ttn->time_of_tara  = ttn->dt_of_tara.time().toString("hh:mm:ss");
    ttn->tara_platforma  = seq_id;
    ttn->field_from_car  = car->num_field;

    //seqDebug () << "real_rup_tara: " << ttn->real_rup_tara;

    async_func_ptr->wrap_async_ex( update_ttn_error_message, "Error updating ttn tara", [&ttn, this]{ async_func_ptr->async_update(ttn); });
}

bool MainSequence::isPureBruttoWeight(const QVariantMap& bill) const throw (MainSequenceException)
{
    if ( memberValue<uint>("bruttoWeight", bill) == 0 ) return true;
    if ( memberValue<uint>("kagat", bill) == 0 ) return false;

    throw MainSequenceException(confuse_brutto_tara_error_message, "confused brutto with tara");
}

bool MainSequence::checkDeltaForReweights(int prev_weight, int weight) const
{
    return qAbs( prev_weight - weight ) < get_setting<int>("brutto_delta_between_reweights", app_settings);
}

bool MainSequence::isWeightCorrect(int w) const
{
    return w >= 0 && tags[alho_settings.weight_stable.tag_name]->func(alho_settings.weight_stable.method_name).toBool();
}

void MainSequence::brutto(QVariantMap & bill, qx::dao::ptr<t_cars> car, const MifareCard& card) const throw(MainSequenceException)
{
    auto ttn = async_func_ptr->wrap_async_ex(fetch_ttn_error_message, "fetching ttn failed!!!",
                                  [&bill, this]{return async_func_ptr->async_fetch<t_ttn>(bill["billNumber"].toUInt());});

    repairBeetFieldCorrectnessIfNeeded(bill, ttn);

    int weight = getWeight();

    //seqDebug () << "weight_value: "<<weight;

    if ( !isWeightCorrect( weight ) ) {
        throw MainSequenceException(weight_not_stable_message, "brutto: weights dont stable!" );
    }

    if ( !isPureBruttoWeight(bill) ){
        if ( !checkDeltaForReweights(memberValue<uint>("bruttoWeight", bill), weight) )  {
            throw MainSequenceException(brutto_rupture_failed_message,
                                    "reweight: brutto rup failed!: prevWeight: " +
                                     memberValue<QString>("bruttoWeight", bill)  +
                                    " cur: " + QString::number(weight)  + " max_delta: " +
                                    get_setting<QString>("brutto_delta_between_reweights", app_settings));
        }
        //seqDebug ()<<"reweight!";

        setMemberValue("flags", 0, true, bill );
        setMemberValue("bruttoWeight", weight, bill);
        setMemberValue("dateOfBrutto", QDateTime::currentDateTime(), bill);


    }
    else {
        //seqDebug() << "weight!";

        setMemberValue("bruttoWeight", weight, bill);
        setMemberValue("dateOfBrutto", QDateTime::currentDateTime(), bill);

        processChemicalAnalysis( bill, ttn );
        processFreeBum( bill, ttn, car );
    }


    updateBruttoValues(bill, ttn, card);

}


void MainSequence::tara(QVariantMap & bill, qx::dao::ptr<t_cars> car) const throw (MainSequenceException)
{
    int weight = getWeight();

    if ( !isWeightCorrect( weight ) ) {
        throw MainSequenceException(weight_not_stable_message, "tara: weights dont stable!" );
    }

    qx::dao::ptr<t_ttn> ttn;

    if ( !isPureTaraWeight(bill) ) {
        seqWarning()<< "tara reweiting!";

        ttn = ttnByDriver( carCodeFromDriver( memberValue<uint>("driver", bill) ) );

        if (!checkDeltaForReweights(ttn->brutto, weight)) {
            throw MainSequenceException(tara_rupture_failed_message,
                                    "reweight: tara rup failed!: prevWeight: " +
                                    QString::number(ttn->brutto)  + " cur: " + QString::number(weight)  + " max_delta: " +
                                    get_setting<QString>("brutto_delta_between_reweights", app_settings));
        }
        ttn->tara = weight;
        ttn->dt_of_tara = QDateTime::currentDateTime();

        processDrivingTime(ttn, car);

        repairBumCorrectnessIfNeeded(ttn);

        processTaraRupture(ttn, car);

        updateTaraValues(bill, ttn, car, false);

        printFinishReport(ttn, car);
    }
    else  {
        //seqDebug( ) << "tara weight!!!";

        ttn = async_func_ptr->wrap_async_ex(fetch_ttn_error_message, "fetching ttn failed!!!",
                                  [&bill, this]{return async_func_ptr->async_fetch<t_ttn>(bill["billNumber"].toUInt());});
        checkBum(bill);
        checkLaboratory(bill, car);
        checkKagat(bill);

        ttn->tara = weight;
        ttn->dt_of_tara = QDateTime::currentDateTime();

        clearBumQueue(ttn);

        processDrivingTime(ttn, car);

        repairBumCorrectnessIfNeeded(ttn);

        processTaraRupture(ttn, car);

        updateTaraValues(bill, ttn, car, true);

        printFinishReport(ttn, car);

        clearWorkBillData(bill);

        qx::dao::ptr<t_ttn> new_ttn = makeNewTask(car, bill);
        if (new_ttn) {
            setMemberValue("billNumber", new_ttn->num_nakl, bill);
            setMemberValue("numField", new_ttn->field, bill);
            printStartReport(new_ttn, car);
        }

    }
}

void MainSequence::clearBumQueue(qx::dao::ptr<t_ttn> ttn) const throw(MainSequenceException)
{
    int bum = ttn->bum;

    if (bum != 99) {
        bum = bum * 10 + ttn->bum_platforma;
    }

    async_func_ptr->wrap_async_ex(clear_bum_queue_error, "clear bum queue error",
                [&ttn, this, &bum]{ async_func_ptr->async_call_query("update t_bum set queue = GREATEST(queue - 1, 0) where id=" + QString::number(bum) + ";") ;});

}

qx::dao::ptr<t_ttn> MainSequence::ttnByDriver( int drv )const throw (MainSequenceException)
{
    auto ret = async_func_ptr->wrap_async_ex(get_ttn_by_driver_tara0_error_message, "getting ttn by driver with zero tara failed!",
                            [&drv, this]{ return async_func_ptr->async_fetch_by_query<t_ttn>(
                                qx_query().where("driver").isEqualTo(drv).and_("brutto").isNotEqualTo(0).and_("tara").isEqualTo(0)); });

    if ( ret ) {
        seqWarning() << "Corrupted data on card!. Maybe dispatcher made task before tara finished!";
        return ret;
    }

    ret = async_func_ptr->wrap_async_ex(get_last_ttn_by_driver_error_message, "getting last ttn by driver failed!",
                                [&drv, this]{ return async_func_ptr->async_fetch_by_query<t_ttn>(
                                qx_query().where("driver").isEqualTo(drv).and_("dt_of_tara").
                                    in_Select("select MAX(dt_of_tara) from t_ttn where driver=" + QString::number(drv)));});

    return ret;
}

bool MainSequence::isPureTaraWeight(const QVariantMap &bill) const throw(MainSequenceException)
{
    if ( memberValue<int>("bruttoWeight", bill) > 0 ) return true;
    if ( memberValue<int>("realNumField", bill) == 0 ) return false;

    throw MainSequenceException(forget_brutto_on_tara_error_message, "forget for brutto!" );
}

void MainSequence::onDisappearOnWeight(const QString& )
{
    seqDebug() << "something disappeared on weight!!!!";

    on_weight = false;

    //qDebug( )     << "1";

    tags[alho_settings.reader.name]->func( alho_settings.reader.do_off );

    //qDebug( )     << "2";

    async_func_ptr->terminate();
    //qDebug( )     << "3";
    if ( wake_timer.isActive() ) {
        //qDebug( )     << "4";
        wake_timer.stop();
        //qDebug( )     << "5";
        wakeUp();
        //qDebug( )     << "6";
    }
    //qDebug( )     << "7";

//    printOnTablo(greeting_message);
//    setLightsToGreen();
}

void MainSequence::checkBum( QVariantMap& bill )const throw(MainSequenceException)
{
    if ( memberValue<int>("bum", bill) == 99 && memberValue<int>("bumFact", bill) == 0 ) {
        setMemberValue("bumFact", 99, bill);
        setMemberValue("kagat", 99, bill);
        //wrap_async_ex( update_bum_queue_error, "Error updating bum queue", [&bum, this]{ async_update(bum); });
    }
}

void MainSequence::checkLaboratory( const QVariantMap& bill , qx::dao::ptr<t_cars> car)const throw(MainSequenceException)
{
    if ( memberValue<QBitArray>("flags", bill).at(2) && !memberValue<QBitArray>("flags", bill).at(3) ) {
        car->block = 1;
        async_func_ptr->wrap_async_ex( blocking_car_for_lab_error, "Error blocking car which wasnt in lab", [&car, this]{ async_func_ptr->async_update(car); });

        throw MainSequenceException (car_dont_was_in_lab, "car dont was in lab");
    }
}

void MainSequence::checkKagat(const QVariantMap& bill)const throw(MainSequenceException)
{
    if ( memberValue<int>("kagat", bill) ==0 )
        throw MainSequenceException(car_has_not_been_unloaded, "car was not unloaded!");

    qx::dao::ptr<t_kagat> kagat = async_func_ptr->wrap_async_ex(getting_kagat_error, "cant get kagat",
                    [&bill, this]{ return async_func_ptr->async_fetch<t_kagat>(memberValue<int>("kagat", bill)) ;});

    if (!kagat->state) {
        throw MainSequenceException(kagat_was_closed_error, "kagat was closed!");
    }

}


void MainSequence::processDrivingTime(qx::dao::ptr<t_ttn> ttn, qx::dao::ptr<t_cars> car) const throw (MainSequenceException)
{
    ushort rup      = dateTimeToTimeShit(ttn->time_return) - dateTimeToTimeShit(ttn->date_time) / 60; /*to hours*/

    if ( car->vremja_na_hodku > rup ) {
        ttn->time_return = timeShitToDateTime( dateTimeToTimeShit(ttn->date_time) + car->vremja_na_hodku * 60 );
    }
}


bool MainSequence::printStartReport(const qx::dao::ptr<t_ttn> & ttn, const qx::dao::ptr<t_cars> & car) const throw(MainSequenceException)
{
    ReportsManager m(app_settings, *async_func_ptr, *convience_func_ptr);

    qx::dao::ptr<t_field> field = async_func_ptr->wrap_async_ex(cant_get_field_when_printing, "cant get field " + QString::number(ttn->field) + " when printing",
                                [&ttn, this]{return async_func_ptr->async_fetch<t_field>( ttn->field ); });

    return m.printReport(ttn, car, field, get_setting<QString>("start_report_file_name", app_settings));
}

bool MainSequence::printFinishReport( const qx::dao::ptr<t_ttn>& ttn, const qx::dao::ptr<t_cars>& car) const throw(MainSequenceException)
{
    ReportsManager m(app_settings, *async_func_ptr, *convience_func_ptr);

    qx::dao::ptr<t_field> field = async_func_ptr->wrap_async_ex(cant_get_field_when_printing, "cant get field " + QString::number(ttn->real_field) + " when printing",
                                [&ttn, this]{return async_func_ptr->async_fetch<t_field>( ttn->real_field ); });

    return m.printReport(ttn, car, field, get_setting<QString>("finish_report_file_name", app_settings));

}

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
    qx::dao::ptr<t_bum_state_log> bum_log = async_func_ptr->wrap_async_ex( cant_get_bum_state_log_message, "cant get bum state log",
        [&q, this]{ return async_func_ptr->async_exec_query<t_bum_state_log>(q, false) ;});



    if (bum_log) return false;

    qx::dao::ptr<t_bum> bum_ptr = async_func_ptr->wrap_async_ex(cant_get_bum_message, "cant get bum: " + QString::number(bum),
                        [&bum, this]{ return async_func_ptr->async_fetch<t_bum>(bum);});

    return static_cast<bool> (bum_ptr->state);
}


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






    int mid_tara = async_func_ptr->wrap_async_ex( error_getting_mid_tara_message, "error getting mid tara",
                                [this, &ttn, &car, &count]{  return async_func_ptr->async_call_query<int>(
                                    "select avg(tara) from (select tara from t_ttn where tara!=0 and num_nakl!="+
                                    QString::number(ttn->num_nakl)+
                                    " and car="+QString::number(car->id)+
                                    " order by -dt_of_tara limit "+count+") as temp_table;");});




    ttn->rup_tara           = mid_tara * percent.toUInt() / 100;
    ttn->real_rup_tara      = qAbs(mid_tara - static_cast<int>(ttn->tara));

    //seqDebug () << "mid_tara: "<<mid_tara;
}



void MainSequence::clearWorkBillData(QVariantMap & bill) const
{
    const QDateTime zero_date_time = timeShitToDateTime(0);

    setMemberValue("billNumber"    ,0, bill);
    setMemberValue("flags"         ,QBitArray(16), bill);
    setMemberValue("numField"      ,0, bill);
    setMemberValue("realNumField"  ,0, bill);
    setMemberValue("numLoader"     ,0, bill);
    setMemberValue("dateOfLoad"    ,zero_date_time, bill);
    //setMemberValue("driver"        ,0, bill);
    setMemberValue("pointOfAnal"   ,0, bill);

    setMemberValue("bruttoWeight"  ,0, bill);
    setMemberValue("dateOfBrutto"  ,zero_date_time, bill);
    setMemberValue("taraWeight"    ,0, bill);
    //setMemberValue("normTaraWeight",0, bill);
    setMemberValue("dateOfTara"    ,zero_date_time, bill);

    setMemberValue("impurity"      ,0, bill);
    setMemberValue("shugarContent" ,0, bill);
    setMemberValue("greenWeight"   ,0, bill);
    setMemberValue("bum"           ,0, bill);
    setMemberValue("bumFact"       ,0, bill);
    setMemberValue("kagat"         ,0, bill);
    setMemberValue("dateOfUnload"  ,zero_date_time, bill);
}


qx::dao::ptr<t_ttn> MainSequence::makeNewTask(qx::dao::ptr<t_cars> car, const QVariantMap& bill) const throw (MainSequenceException)
{
    if (car->num_field == 0) return qx::dao::ptr<t_ttn>();

    QDateTime end_time = timeShitToDateTime( car->vremja_na_hodku*60 + dateTimeToTimeShit(QDateTime::currentDateTime())  );

    qx::dao::ptr<t_ttn> ttn = qx::dao::ptr<t_ttn>(new t_ttn);

    ttn->date_time      = QDateTime::currentDateTime();
    ttn->car            = car->id;
    ttn->field          = car->num_field;
    ttn->driver         = memberValue<uint>("driver",bill);
    ttn->time_return    = end_time;
    ttn->copy           = 0;
    ttn->time_of_return = end_time.toString("hh:mm:ss");
    ttn->loader         = car->num_loader;

    //qx::dao::insert(ttn, &database);

    async_func_ptr->wrap_async_ex(error_make_new_task, "error make new task", [this, &ttn]{return async_func_ptr->async_insert(ttn);});

    return ttn;
}

void MainSequence::setLightsToRed()
{
    //tags["do1"]->func("writeMethod", Q_ARG(QVariant, 1));
    //tags["do2"]->func("writeMethod", Q_ARG(QVariant, 0));

    tags[alho_settings.red_light.tag_name]->func(alho_settings.red_light.method_name, Q_ARG(QVariant, 1));
    tags[alho_settings.green_light.tag_name]->func(alho_settings.green_light.method_name, Q_ARG(QVariant, 0));

}

void MainSequence::setLightsToGreen()
{
//    tags["do1"]->func("writeMethod", Q_ARG(QVariant, 0));
//    tags["do2"]->func("writeMethod", Q_ARG(QVariant, 1));
    tags[alho_settings.red_light.tag_name]->func(alho_settings.red_light.method_name, Q_ARG(QVariant, 0));
    tags[alho_settings.green_light.tag_name]->func(alho_settings.green_light.method_name, Q_ARG(QVariant, 1));
}




void MainSequence::processPerimeter() const throw (MainSequenceException)
{
    if ( !get_setting<bool>("perimeter_control", app_settings) ) return;

//    if ( tags["di1"]->func("readMethod").toBool() || tags["di2"]->func("readMethod").toBool() )
//        throw MainSequenceException(perimeter_control_failed, "perimeter_control_failed!");

    if ( tags[ alho_settings.perim_in.tag_name ]->func(alho_settings.perim_in.method_name).toBool() ||
         tags[ alho_settings.perim_out.tag_name ]->func(alho_settings.perim_out.method_name).toBool() )
        throw MainSequenceException(perimeter_control_failed, "perimeter_control_failed!");

}







