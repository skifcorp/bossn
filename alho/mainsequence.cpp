#include "mainsequence.h"
#include "mifarecard.h"
#include "settingstool.h"
#include "func.h"

#include "dbstructs.h"
#include "warnmessages.h"
#include "reportsmanager.h"
#include "codeshacks.h"
#include "conviencefuncs.h"

//#include "datetimehack.h"

//#include "reports.h"

#include <QBitArray>



const QString brutto_finish_weight_message                  = QT_TRANSLATE_NOOP("MainSequence", "%1 kg");
const QString brutto_finish_lab_message                     = QT_TRANSLATE_NOOP("MainSequence", "Lab(%1)");
const QString brutto_finish_bum_message                     = QT_TRANSLATE_NOOP("MainSequence", "BUM %1");


void MainSequence::setSettings(const QVariantMap & )
{

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


/*

template <class Callable, class... Args>
void async_call(Callable c, Args ... args)
{
    auto f = async(std::launch::async, c, args...);

    while (!f.is_ready()) {
        qApp->processEvents();
    }
}*/

void MainSequence::printOnTablo(const QString & s)
{
    tags["tablo"]->func("print", Q_ARG(const QVariant&, QVariant(s)));
}

int MainSequence::getWeight() const
{
    return tags["weight1_1"]->func("readMethod").toInt();
}


MainSequence::MainSequence(Tags & t, const QVariantMap& s):tags(t), app_settings(s), on_weight(false)
{
    printOnTablo(greeting_message);
    setLightsToGreen();

    qx::QxSqlDatabase::getSingleton()->setTraceSqlQuery(false);
    qx::QxSqlDatabase::getSingleton()->setTraceSqlRecord(false);

    qx::QxSqlDatabase::getSingleton()->setDriverName(get_setting<QString>("database_driver", app_settings));
    qx::QxSqlDatabase::getSingleton()->setDatabaseName(get_setting<QString>("database_name", app_settings));
    qx::QxSqlDatabase::getSingleton()->setHostName(get_setting<QString>("database_host", app_settings));
    qx::QxSqlDatabase::getSingleton()->setUserName(get_setting<QString>("database_user", app_settings));
    qx::QxSqlDatabase::getSingleton()->setPassword(get_setting<QString>("database_password", app_settings));


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

void MainSequence::onAppearOnWeight(const QString& )
{
    qDebug() << "something appeared on weight!!!!";
    on_weight = true;

    printOnTablo(apply_card_message);

    setLightsToRed();

    tags["reader1"]->func("doOn");

    QByteArray card_code = get_setting<QByteArray>("card_code" , app_settings);
    uint data_block      = get_setting<uint>      ("data_block", app_settings);

    while(on_weight) {
        ActivateCardISO14443A act = tags["reader1"]->func("activateIdleA").value<ActivateCardISO14443A>();
        MifareCard card(tags["reader1"], act);

        if ( !card.active() ) {
            qDebug() << "card not active!!!";            
            sleepnbtm(); continue;
        }

        qDebug()<< "card active!";

        try {
            card.autorize(card_code, data_block);

            processPerimeter();

            QVariantMap bill = card.readStruct(bill_conf(app_settings));

            if (!checkMember("billNumber", bill, 0) || !checkMember("driver", bill, 0)) {
                throw MainSequenceException(card_is_empty_error_message, "bill is empty");
            }

            auto car = wrap_async_ex(fetch_car_error_message, "fetching car failed!!!: driver: " + memberValue<QString>("driver", bill),
                                  [&bill, this]{return async_fetch<t_cars>( carCodeFromDriver( memberValue<uint>("driver", bill) ) );});


            if (car->block) {
                throw MainSequenceException(car_blocked_message, "car is blocked!!!");
            }

            QString platform_type = detectPlatformType(bill);

            if (platform_type == "brutto" ) {
                qDebug() << "brutto...";

                brutto(bill, car, card);

                card.writeStruct(bill_conf(app_settings), bill);

                printOnTablo( bruttoFinishMessage(bill) );                                           
                sleepnb( get_setting<int>("brutto_finish_pause", app_settings) );
                printOnTablo( apply_card_message );
                continue;
            }
            else if (platform_type == "tara" ) {
                qDebug() << "tara...";

                tara(bill, car);
                card.writeStruct(bill_conf(app_settings), bill);
                printOnTablo( taraFinishMessage() );
                sleepnb( get_setting<int>("brutto_finish_pause", app_settings) );
                printOnTablo( apply_card_message );
                continue;
            }
        }
        catch (MifareCardAuthException& ex) {
            qWarning() << "auth_exeption! "<<ex.message();
            sleepnbtmerr(card_autorize_error_message, apply_card_message);
            continue;
        }
        catch (MifareCardException& ex) {
            qWarning() << "card_exception! "<<ex.message();
            sleepnbtmerr(ex.message(), apply_card_message);
            continue;
        }
        catch (MainSequenceException& ex) {
            qWarning()<<"sequence_exception: " << ex.adminMessage();

            sleepnbtmerr(ex.userMessage(), apply_card_message);
            continue;
        }
    }

}

void MainSequence::repairBeetFieldCorrectnessIfNeeded(QVariantMap & bill, qx::dao::ptr<t_ttn> ttn) const throw()
{  
    //qDebug () << "before!!!";
    if ( memberValue<uint>("realNumField", bill) == 0 || !wrap_async( [&bill, this]{
                                    return async_fetch<t_field>( memberValue<uint>("realNumField", bill) );
                                 } ) )  {

        setMemberValue("realNumField", memberValue<uint>("numField", bill), bill);
        ttn->real_field = memberValue<uint>("numField", bill);
        qDebug () << "field repared: "<<ttn->real_field;

/*        try {
            //async_update(ttn);
        }
        catch(MysqlException& ex) {
            qWarning()<< ex.databaseText() + " " + ex.driverText();
        }*/
    }
    //qDebug () << "after!!!";
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
        async_count<t_ttn>( count, q );

        return count;
    }
    catch (MysqlException& ex) {
        qWarning() << "cant get count of ttns for chemical analysis!!! field: "<<field_num;
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
        const_ = wrap_async_ex( "error for kontragent type when doint chemical analysis typ=0", QString(),                                
                                [this]{return async_fetch<t_const>(get_setting<QString>("corpotare_check_period_name", app_settings));});
    }
    else if ( typ == 3 ) {
        const_ = wrap_async_ex( "error for kontragent type when doint chemical analysis typ=3", QString(),
                                [this]{return async_fetch<t_const>(get_setting<QString>("farmer_check_period_name", app_settings)); });
    }
    else {
        //qWarning()<< "error for kontragent type when doint chemical analysis";
        throw MainSequenceException("error for kontragent type when doint chemical analysis", QString());
    }

    return const_->value.toUInt();
}

bool MainSequence::checkForNeedDatabaseConstAnalisys(long count, long kontrag) const throw ()
{
    if (count + 1 == 1) return true; //first car always go for analysis

    try {
        auto kontr = wrap_async_ex( "cant get kontr of ttns for chemical analysis!!! kontrag: " + QString::number(kontrag),
                                     QString(),
                                    [&kontrag, this]{return async_fetch<t_kontr>(kontrag);} );

        uint period = kontr->period;

        if ( period == 0 ) {
            period = wrap_async_ex("error for getting const for type:" + QString::number(kontr->type), QString(),
                                   [&kontr, this]{return getAnalisysPeriodFromStorage(kontr->type);} );
        }

        uint carInPeriod = kontr->car_in_period;
        if (carInPeriod == 0 ) {
            carInPeriod = 1;
            qWarning() << "error for getting carInPeriod. Kontragent dont have corrent chemical analisys params";
        }

        if ( period == 0 )  {
            period = 1;
            qWarning() << "error for getting period. Kontragent dont have corrent chemical analisys params";
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

    qDebug() << "analisys alho: "<<alho<<" cars_count: "<<count;

    if ( alho == "discrete"  ) {
        if ( !checkForNeedDiscreteAnalisys(count) ) {
            qDebug () << " dont needed...";
            return;
        }
    }
    else if (alho == "database_const") {        
        if ( !checkForNeedDatabaseConstAnalisys( count, kontrCodeFromField( memberValue<uint>("realNumField", bill) ) ) ) {
            qDebug () << " dont needed...";
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
        qx::dao::ptr<t_const> const_ = wrap_async_ex(get_backboard_bum_weight_const_error, "error getting const for backboard bum",
                        [this]{return async_fetch<t_const>(get_setting<QString>("bum11_name", app_settings));});

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

    qx::dao::ptr<t_bum> bum = wrap_async_ex(get_free_bum_error, "Error getting free bum1",
        [&bums_where_clause, &q1, this]{ return async_exec_query<t_bum>(q1, false);});

    //qDebug() << "----------------> BUM_ID: "<<bum->id;

    if ( !bum ) {
        bum = wrap_async_ex(get_free_bum_error, "Error getting free bum2",
            [&bums_where_clause, &q2, this]{ return async_exec_query<t_bum>(q2);});
    }

    bum->queue += 1;
    wrap_async_ex( update_bum_queue_error, "Error updating bum queue", [&bum, this]{ async_update(bum); });

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
    ttn->driver           = carCodeFromDriver( memberValue<int>("driver", bill) ) ;
    ttn->bum              = memberValue<int>("bum", bill);
    ttn->routed_to_lab    = memberValue<QBitArray>("flags", bill).at(2);
    ttn->num_kart         = byteArrayToString (card.uid());
    ttn->copy             = 0;
    ttn->time_of_brutto   = ttn->dt_of_brutto.time().toString("hh:mm:ss");
    ttn->brutto_platforma = 88;


    wrap_async_ex( update_ttn_error_message, "Error updating ttn brutto", [&ttn, this]{ async_update(ttn); });
}

void MainSequence::updateTaraValues(QVariantMap& bill, qx::dao::ptr<t_ttn> ttn, qx::dao::ptr<t_cars> car, bool pure_weight) const throw (MainSequenceException)
{
    if ( pure_weight ) {
        ttn->real_bum      = memberValue<int>("bumFact", bill);
        ttn->kagat         = memberValue<int>("kagat", bill);
        ttn->dt_of_unload  = memberValue<QDateTime>("dateOfUnload", bill);
        ttn->was_in_lab    = memberValue<QBitArray>("flags", bill).at(3);

        qDebug () << "bum: " << ttn->bum;
    }

    ttn->copy          = 0;
    ttn->time_of_tara  = ttn->dt_of_tara.time().toString("hh:mm:ss");
    ttn->tara_platforma  = 99;
    ttn->field_from_car  = car->num_field;

    wrap_async_ex( update_ttn_error_message, "Error updating ttn tara", [&ttn, this]{ async_update(ttn); });
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

void MainSequence::brutto(QVariantMap & bill, qx::dao::ptr<t_cars> car, const MifareCard& card) const throw(MainSequenceException)
{
    auto ttn = wrap_async_ex(fetch_ttn_error_message, "fetching ttn failed!!!",
                                  [&bill, this]{return async_fetch<t_ttn>(bill["billNumber"].toUInt());});

    repairBeetFieldCorrectnessIfNeeded(bill, ttn);

    int weight = getWeight();

    qDebug () << "weight_value: "<<weight;

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
        setMemberValue("flags", 0, true, bill );
        qDebug ()<<"reweight!";
    }

    setMemberValue("bruttoWeight", weight, bill);
    setMemberValue("dateOfBrutto", QDateTime::currentDateTime(), bill);

    processChemicalAnalysis( bill, ttn );

    processFreeBum( bill, ttn, car );

    updateBruttoValues(bill, ttn, card);

    //check for replace field
    //checkBeetFieldCorrectness(bill);
    //check if bill presents in db

    //weight or reweight:
    //for reweight checking delta with old and new weights
    //write weight and datetime
    //detect chemical analysis
    //detect free bum
    //updating mysql database
    //write to card
    //do sound
    //print message for tablo
}


void MainSequence::tara(QVariantMap & bill, qx::dao::ptr<t_cars> car) const throw (MainSequenceException)
{
    //check for reweight tara and time for route and print

    //preparing:
    //check brutto
    //check bum
    //check kagat
    //check if kagat works

    //weight or reweight:
    //check if tara < brutto
    //for reweight checking delta with old and new weights
    //clear bum queque
    //write tara to db
    //analyze route time
    //printing
    //creating new task
    //



    int weight = getWeight();

    if ( !isWeightCorrect( weight ) ) {
        throw MainSequenceException(weight_not_stable_message, "tara: weights dont stable!" );
    }

    qx::dao::ptr<t_ttn> ttn;

    if ( !isPureTaraWeight(bill) ) {
        qWarning()<< "tara reweiting!";

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
        ttn = wrap_async_ex(fetch_ttn_error_message, "fetching ttn failed!!!",
                                  [&bill, this]{return async_fetch<t_ttn>(bill["billNumber"].toUInt());});
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

    wrap_async_ex(clear_bum_queue_error, "clear bum queue error",
                [&ttn, this, &bum]{ async_call_query("update t_bum set queue = queue - 1 where id=" + QString::number(bum) + ";") ;});

}

qx::dao::ptr<t_ttn> MainSequence::ttnByDriver( int drv )const throw (MainSequenceException)
{
    auto ret = wrap_async_ex(get_ttn_by_driver_tara0_error_message, "getting ttn by driver with zero tara failed!",
                            [&drv, this]{ return async_fetch_by_query<t_ttn>(
                                qx_query().where("driver").isEqualTo(drv).and_("brutto").isNotEqualTo(0).and_("tara").isEqualTo(0)); });

    if ( ret ) {
        qWarning() << "Corrupted data on card!. Maybe dispatcher made task before tara finished!";
        return ret;
    }

    ret = wrap_async_ex(get_last_ttn_by_driver_error_message, "getting last ttn by driver failed!",
                                [&drv, this]{ return async_fetch_by_query<t_ttn>(
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
    qDebug() << "something disappeared on weight!!!!";

    on_weight = false;

    tags["reader1"]->func("doOff");
    //tags["tablo"]->func("print", Q_ARG(const QVariant&, QVariant("Zaidkte na vagu")));
    printOnTablo(greeting_message);
    setLightsToGreen();   
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
        wrap_async_ex( blocking_car_for_lab_error, "Error blocking car which wasnt in lab", [&car, this]{ async_update(car); });

        throw MainSequenceException (car_dont_was_in_lab, "car dont was in lab");
    }
}

void MainSequence::checkKagat(const QVariantMap& bill)const throw(MainSequenceException)
{
    if ( memberValue<int>("kagat", bill) ==0 )
        throw MainSequenceException(car_has_not_been_unloaded, "car was not unloaded!");

    qx::dao::ptr<t_kagat> kagat = wrap_async_ex(getting_kagat_error, "cant get kagat",
                    [&bill, this]{ return async_fetch<t_kagat>(memberValue<int>("kagat", bill)) ;});

    if (!kagat->state) {
        throw MainSequenceException(kagat_was_closed_error, "kagat was closed!");
    }

}


void MainSequence::processDrivingTime(qx::dao::ptr<t_ttn> ttn, qx::dao::ptr<t_cars> car) const throw (MainSequenceException)
{
//    ushort rup_real = dateTimeToTimeShit(ttn->dt_of_brutto) - dateTimeToTimeShit(ttn->date_time) / 60; /*to hours*/
    ushort rup      = dateTimeToTimeShit(ttn->time_return) - dateTimeToTimeShit(ttn->date_time) / 60; /*to hours*/

    if ( car->vremja_na_hodku > rup ) {
        ttn->time_return = timeShitToDateTime( dateTimeToTimeShit(ttn->date_time) + car->vremja_na_hodku * 60 );
    }
}

#if 0
void MainSequence::configureReportContext(const qx::dao::ptr<t_ttn>& ttn, const qx::dao::ptr<t_cars>& car, QVariantMap& ctx) const throw (MainSequenceException)
{
    qx::dao::ptr<t_field> field = wrap_async_ex(cant_get_field_when_printing, "cant get field when printing",
                            [&ttn, this]{return async_fetch<t_field>( ttn->real_field ); });

    qx::dao::ptr<t_kontr> kontr = wrap_async_ex(cant_get_kontr_when_printing, "cant get kontr when printing",
                            [&ttn, this]{return async_fetch<t_kontr>( kontrCodeFromField( ttn->real_field  ) ); });


    qx::dao::ptr<t_const> base_firm         = getConst(get_setting<QString>("base_firm_name"   , app_settings));
    qx::dao::ptr<t_const> dont_check_time   = getConst(get_setting<QString>("dont_check_time_name", app_settings));
    qx::dao::ptr<t_const> disp_phone        = getConst(get_setting<QString>("disp_phone_name", app_settings));

  struct var_instance {
        QString var_name;
        QString typ_name;
        void * var;
    };
    QList<var_instance> vars = QList<var_instance>{
        var_instance{"t_ttn", "t_ttn", ttn.data()},
        var_instance{"t_cars", "t_cars", car.data()},
        var_instance{"t_field", "t_field", field.data()},
        var_instance{"t_kontr", "t_kontr", kontr.data()},
        var_instance{"base_firm", "t_const", base_firm.data()},
        var_instance{"dont_check_time", "t_const", dont_check_time.data()},
        var_instance{"disp_phone", "t_const", disp_phone.data()}
     };

    QTextCodec * co = QTextCodec::codecForName("Windows-1251");
    //QVariantMap ctx;
    for( auto iter = vars.begin(); iter != vars.end(); ++iter ) {
        IxClass * c = QxClassX::getClass( iter->typ_name );
        //qDebug() << "name: "<<c->getName();
        IxDataMemberX * m = c->getDataMemberX();
        for ( long i = 0; i < m->count(); ++i  ) {
            IxDataMember * dm = m->get(i);
            qDebug () << "sql_type: "<<dm->getSqlType();
            ctx[iter->var_name + "_" + dm->getName()] = dm->toVariant(  iter->var  );
        }
    }

}

#endif

bool MainSequence::printStartReport(const qx::dao::ptr<t_ttn> & ttn, const qx::dao::ptr<t_cars> & car) const throw(MainSequenceException)
{
#if 0
    Reports r ( get_setting<QString>("start_report_file_name", app_settings) );

    //ttn->real_field = 101;

    QVariantMap ctx;

    configureReportContext(ttn, car, ctx);

    return r.print(ctx);
#endif
    ReportsManager m(app_settings);
    return m.printReport(ttn, car, get_setting<QString>("start_report_file_name", app_settings));
}

bool MainSequence::printFinishReport( const qx::dao::ptr<t_ttn>& ttn, const qx::dao::ptr<t_cars>& car) const throw(MainSequenceException)
{
#if 0
    Reports r ( get_setting<QString>("finish_report_file_name", app_settings) );

    //ttn->real_field = 101;

    QVariantMap ctx;

    configureReportContext(ttn, car, ctx);

    return r.print(ctx);
#endif
    ReportsManager m(app_settings);
    return m.printReport(ttn, car, get_setting<QString>("finish_report_file_name", app_settings));

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
    qx_query q;
    q.where("date_time").isGreaterThanOrEqualTo(date_from).and_("date_time").isLessThanOrEqualTo(date_to).and_("bum").isEqualTo(QVariant::fromValue<long>(bum));
    qx::dao::ptr<t_bum_state_log> bum_log = wrap_async_ex( cant_get_bum_state_log_message, "cant get bum state log",
                            [&q, this]{ return async_fetch_by_query<t_bum_state_log>(q) ;});

    if (bum_log) return false;

    qx::dao::ptr<t_bum> bum_ptr = wrap_async_ex(cant_get_bum_message, "cant get bum",
                        [&bum, this]{ return async_fetch<t_bum>(bum);});

    return static_cast<bool> (bum_ptr->state);
}
#if 0
qx::dao::ptr<t_const> MainSequence::getConst(const QString & k) const throw(MainSequenceException)
{
    auto const_ = wrap_async_ex( cant_get_const_message + ": " + k, "cant get const " + k,
                                [this, &k]{return async_fetch<t_const>(k);});
    return const_;
}
#endif

void MainSequence::processTaraRupture(qx::dao::ptr<t_ttn> ttn, qx::dao::ptr<t_cars> car) const throw(MainSequenceException)
{
    QString percent = getConst( get_setting<QString>("tara_percent_name", app_settings) )->value ;

    QString count;
    if ( car->amount_of_car_for_middle_tara < 0 ) {
        count = getConst( get_setting<QString>("tara_cars_mid_count_name", app_settings) )->value ;
    }
    else {
        count = QString::number( car->amount_of_car_for_middle_tara );
    }



    int mid_tara = wrap_async_ex( error_getting_mid_tara_message, "error getting mid tara",
                                [this, &ttn, &car, &count]{  return async_call_query<int>(
                                    "select avg(tara) from (select tara from t_ttn where tara!=0 and num_nakl!="+
                                    QString::number(ttn->num_nakl)+
                                    " and car="+QString::number(car->id)+
                                    " order by -dt_of_tara limit "+count+") as temp_table;");});

    ttn->rup_tara           = mid_tara * percent.toUInt() / 100;
    ttn->real_rup_tara      = qAbs(ttn->tara - mid_tara);
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

    qx::dao::insert(ttn);

    return ttn;
}

void MainSequence::setLightsToRed()
{
    tags["do1"]->func("writeMethod", Q_ARG(QVariant, 1));
    tags["do2"]->func("writeMethod", Q_ARG(QVariant, 0));
}

void MainSequence::setLightsToGreen()
{
    tags["do1"]->func("writeMethod", Q_ARG(QVariant, 0));
    tags["do2"]->func("writeMethod", Q_ARG(QVariant, 1));
}




void MainSequence::processPerimeter() const throw (MainSequenceException)
{
    if ( !get_setting<bool>("perimeter_control", app_settings) ) return;

    if ( tags["di1"]->func("readMethod").toBool() || tags["di2"]->func("readMethod").toBool() )
        throw MainSequenceException(perimeter_control_failed, "perimeter_control_failed!");
}







